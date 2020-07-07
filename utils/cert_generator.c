#include "fourq.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

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
    char start_time[26];
    char end_time[26];
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

    mavlink_authority_certificate_t cert;

    printf("Generation of authority certificate\n");

    CompressedKeyGeneration(cert.secret_key, cert.public_key);

    printf("Enter certificate name: ");
    scanf("%s", cert.certificateName);

    printf("Enter certificate issuer: ");
    scanf("%s", cert.issuer);
    strcpy(cert.subject, cert.issuer);

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
    mavlink_authority_certificate_t authority_certificate;
    mavlink_device_certificate_t device_certificate;

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

    strcpy(device_certificate.info.start_time, asctime(localtime(&start)));
    strcpy(device_certificate.info.end_time, asctime(localtime(&end)));
    strcpy(device_certificate.info.issuer, device_certificate.info.subject);
    device_certificate.info.start_time[25] = 0;
    device_certificate.info.end_time[25] = 0;
    device_certificate.info.issuer[19] = 0;


    CompressedKeyGeneration(device_certificate.secret_key, device_certificate.info.public_key);

    printf("Pubic_key:");
    hex_print(device_certificate.info.public_key, 0, 32);

    printf("Secret_key:");
    hex_print(device_certificate.secret_key, 0, 32);

    uint8_t cert[sizeof(info_t)];
    memcpy(cert, &device_certificate.info, sizeof(info_t));

    SchnorrQ_Sign(authority_certificate.secret_key, authority_certificate.public_key_auth, cert, sizeof(info_t), device_certificate.sign);
    unsigned int valid;
    SchnorrQ_Verify(authority_certificate.public_key_auth, cert, sizeof(info_t), device_certificate.sign, &valid);

    if (valid)
    {
        memcpy(device_certificate.info.public_key_auth, authority_certificate.public_key, sizeof(authority_certificate.public_key));
        fp = fopen("device.cert", "wb");
        fwrite(&device_certificate, sizeof(device_certificate), 1, fp);
        fclose(fp);
        printf("Valid from %s to %s\n",device_certificate.info.start_time,device_certificate.info.end_time);

        return;
    }
    exit(1);
}
