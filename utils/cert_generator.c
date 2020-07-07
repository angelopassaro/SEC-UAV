#include "fourq.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

#define member_size(type, member) sizeof(((type *)0)->member)


void hex_print(uint8_t *pv, uint16_t s, uint16_t len)
{
    uint8_t *p = pv;
    if (NULL == pv)
        printf("NULL");
    else
    {
        unsigned int i;
        for (i = s; i < len; ++i)
            printf("%02x ", p[i]);
    }
    printf("\n\n");
}

typedef struct info_s
{
    uint8_t seq_number;
    uint8_t device_id;
    char device_name[20];
    char subject[20];
    char issuer[20];
    uint8_t public_key[32];
    uint8_t public_key_auth[32];
    float start_time;
    float end_time;
} info_t;

typedef struct mavlink_device_certificate
{
    info_t info;
    uint8_t secret_key[32];
    uint8_t sign[64];
} mavlink_device_certificate_t;

typedef struct mavlink_authority_certificate
{
    char certificateName[20];
    char issuer[20];
    char subject[20];
    uint8_t seq_number;
    uint8_t public_key[32];
    uint8_t secret_key[32];
    uint8_t public_key_auth[32];
} mavlink_authority_certificate_t;

void authorityCertGen(void);
void uavCertGen(void);
void signCertificate(mavlink_device_certificate_t *cert, uint8_t *sk, uint8_t *pk);

int main()
{

    printf("1 - Generate auth cert\n");
    printf("2 - Generate uav cert\n");

    int option = 0;
    while (option == 0 || option > 2)
    {
        printf("Choose a one option\n");

        scanf("%d", &option);
    }

    if (option == 1)
    {
        authorityCertGen();
    }
    else if (option == 2)
    {
        uavCertGen();
    }

    return 0;
}

void authorityCertGen(void)
{

    static mavlink_authority_certificate_t cert;

    printf("Generation of authority certificate\n");

    CompressedKeyGeneration(cert.secret_key, cert.public_key);

    printf("Enter certificate name: ");
    scanf("%s", cert.certificateName);

    printf("Enter certificate issuer: ");
    scanf("%s", cert.issuer);
    strcpy(cert.subject, cert.issuer);
    cert.subject[19] = 0;

    printf("Name: %s \n", cert.certificateName);
    printf("Issuer: %s \n", cert.issuer);
    printf("Subject: %s \n", cert.subject);

    printf("Public_key:");
    hex_print(cert.public_key, 0, 32);

    printf("Secret_key:");
    hex_print(cert.secret_key, 0, 32);

    SchnorrQ_KeyGeneration(cert.secret_key, cert.public_key_auth);
    printf("Public_key_auth:");
    hex_print(cert.public_key_auth, 0, 32);

    cert.seq_number = 0;
    printf("Sequent number: 0x%x\n", cert.seq_number);

    FILE *fp;
    fp = fopen("authority.cert", "wb");
    fwrite(&cert, sizeof(cert), 1, fp);
    fclose(fp);
    return;
}

void uavCertGen()
{
    static mavlink_authority_certificate_t authority_certificate;
    static mavlink_device_certificate_t device_certificate;

    FILE *fp;
    fp = fopen("authority.cert", "rb+");
    fread(&authority_certificate, sizeof(authority_certificate), 1, fp);
    fclose(fp);
    authority_certificate.seq_number += 1;
    fp = fopen("authority.cert", "wb+");
    fwrite(&authority_certificate, sizeof(authority_certificate), 1, fp);
    fclose(fp);

    printf("Loaded authority certificate \n");
    printf("Name: %s\n", authority_certificate.certificateName);
    printf("issuer: %s\n", authority_certificate.issuer);
    printf("Subject: %s\n", authority_certificate.subject);
    printf("Current sequent number %d\n", authority_certificate.seq_number);
    printf("Public_key:");
    hex_print(authority_certificate.public_key, 0, 32);
    printf("Public_key_auth:");
    hex_print(authority_certificate.public_key_auth, 0, 32);
    printf("Secret_key:");
    hex_print(authority_certificate.secret_key, 0, 32);

    printf("Creation of UAV certificate \n");

    device_certificate.info.seq_number = authority_certificate.seq_number;

    int value = 0;
    printf("Enter device ID: ");
    scanf("%d", &value);
    device_certificate.info.device_id = value; // or random value?

    printf("Enter device name: ");
    scanf("%s", device_certificate.info.device_name);

    printf("Enter subjet name: ");
    scanf("%s", device_certificate.info.subject);

    int days = 0;
    time_t start;
    printf("Enter data range(Number of days):\n");
    scanf("%d", &days);

    time(&start);
    struct tm *tm = localtime(&start);
    tm->tm_mday += days;
    time_t end = mktime(tm);

    device_certificate.info.start_time = start;
    device_certificate.info.end_time = end;
    strcpy(device_certificate.info.issuer, authority_certificate.issuer);

    CompressedKeyGeneration(device_certificate.secret_key, device_certificate.info.public_key);

    printf("Sequent number: %d\n", device_certificate.info.seq_number);
    printf("Pubic_key:");
    hex_print(device_certificate.info.public_key, 0, 32);

    printf("Secret_key:");
    hex_print(device_certificate.secret_key, 0, 32);
    memcpy(device_certificate.info.public_key_auth,authority_certificate.public_key_auth,32);

    uint8_t cert[sizeof(info_t)];

    /**
     * wrong order on time
    memcpy(cert, &device_certificate, sizeof(info_t));
    hex_print(cert,0,sizeof(info_t));
    */
    memcpy(&cert[0],&device_certificate.info.seq_number,member_size(info_t, seq_number));
    memcpy(&cert[member_size(info_t, seq_number)],&device_certificate.info.device_id,member_size(info_t, device_id));
    memcpy(&cert[member_size(info_t, seq_number) + member_size(info_t, device_id)],device_certificate.info.device_name,member_size(info_t, device_name));
    memcpy(&cert[member_size(info_t, seq_number) + member_size(info_t, device_id) + member_size(info_t, device_name)],device_certificate.info.subject,member_size(info_t, subject));
    memcpy(&cert[member_size(info_t, seq_number) + member_size(info_t, device_id) + member_size(info_t, device_name) + member_size(info_t, subject)],device_certificate.info.issuer,member_size(info_t, issuer));
    memcpy(&cert[member_size(info_t, seq_number) + member_size(info_t, device_id) + member_size(info_t, device_name) + member_size(info_t, subject) + member_size(info_t, issuer)],device_certificate.info.public_key,member_size(info_t, public_key));
    memcpy(&cert[member_size(info_t, seq_number) + member_size(info_t, device_id) + member_size(info_t, device_name) + member_size(info_t, subject) + member_size(info_t, issuer) + member_size(info_t, public_key)],device_certificate.info.public_key_auth,member_size(info_t, public_key_auth));
    memcpy(&cert[member_size(info_t, seq_number) + member_size(info_t, device_id) + member_size(info_t, device_name) + member_size(info_t, subject) + member_size(info_t, issuer) + member_size(info_t, public_key) + member_size(info_t, public_key_auth)],&device_certificate.info.start_time,member_size(info_t, start_time));
    memcpy(&cert[member_size(info_t, seq_number) + member_size(info_t, device_id) + member_size(info_t, device_name) + member_size(info_t, subject) + member_size(info_t, issuer) + member_size(info_t, public_key) + member_size(info_t, public_key_auth)+ member_size(info_t, start_time)],&device_certificate.info.end_time,member_size(info_t, end_time));

    SchnorrQ_Sign(authority_certificate.secret_key, authority_certificate.public_key_auth, cert, sizeof(info_t), device_certificate.sign);
    unsigned int valid;
    SchnorrQ_Verify(authority_certificate.public_key_auth, cert, sizeof(info_t), device_certificate.sign, &valid);

    if (valid)
    {
        fp = fopen("device.cert", "wb");
        fwrite(&device_certificate, sizeof(mavlink_device_certificate_t), 1, fp);
        fclose(fp);
        printf("Valid from %s to %s\n",asctime(localtime(&start)),asctime(localtime(&end)));
        return;
    }
    exit(1);
}