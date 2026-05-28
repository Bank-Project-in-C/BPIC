

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MIN_CEKILME_MEBLEGI     1.0
#define MAX_CEKILME_MEBLEGI  1000.0


#define MIN_KOCURME_MEBLEGI     0.01
#define MAX_KOCURME_MEBLEGI  5000.0

#define MAX_MEBLAQ_ARTIRMA   10000.0

#define KART_NOMRESI_UZUNLUGU  17


#define MAX_KOD_UZUNLUGU       20


#define XIDMET_SAYI             5




typedef struct {
    char   kart_nomresi[KART_NOMRESI_UZUNLUGU]; 
    double balans;                               
    char   sahib_adi[50];                        
} Kart;


typedef struct {
    int  kod;         
    char ad[30];      
    double min_odenis; 
} Xidmet;


static void buffer_temizle(void)
{
    int c;
    
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}


static void xett_ciz(void)
{
    printf("--------------------------------------------------\n");
}


static double double_oxu(const char *mesaj)
{
    double deger;
    int oxuma_sayi;

    while (1) {
        printf("%s", mesaj);
        oxuma_sayi = scanf("%lf", &deger);
        buffer_temizle(); 

        if (oxuma_sayi == 1) {
            
            return deger;
        }
        
        printf("  [XETA] Zehmet olmasa düzgün rəqəm daxil edin.\n");
    }
}


static void kart_nomresi_oxu(char *hedef)
{
    
    char temp_buf[KART_NOMRESI_UZUNLUGU + 1]; 

    while (1) {
        printf("  Kart nomresi (16 reqem): ");
        
        if (fgets(temp_buf, sizeof(temp_buf), stdin) != NULL) {
           
            size_t uzunlug = strlen(temp_buf);
            if (uzunlug > 0 && temp_buf[uzunlug - 1] == '\n') {
                temp_buf[uzunlug - 1] = '\0';
                uzunlug--;
            } else {
              
                buffer_temizle();
            }

            
            if (uzunlug == 16) {
                
                strncpy(hedef, temp_buf, KART_NOMRESI_UZUNLUGU);
                hedef[KART_NOMRESI_UZUNLUGU - 1] = '\0'; 
                return;
            }
            printf("  [XETA] Kart nomresi tam 16 reqemden ibaret olmalidir.\n");
        }
    }
}


void ode_kommunal(Kart *kart)
{
    
    const Xidmet xidmetler[XIDMET_SAYI] = {
        {1, "Ishiq (Azerenerji)",   1.00},
        {2, "Qaz (Azerigas)",       1.00},
        {3, "Su (Azersu)",          0.50},
        {4, "Internet / TV",        5.00},
        {5, "Mobil telefon",        1.00}
    };

    int     secim;          
    char    musteri_kodu[MAX_KOD_UZUNLUGU]; 
    double  odenis_meblegi;  
    int     secim_gosterici; 

    xett_ciz();
    printf("       KOMMUNAL / XİDMƏT ODENİSİ\n");
    xett_ciz();

    
    printf("  Xidmet seçin:\n");
    for (int i = 0; i < XIDMET_SAYI; i++) {
        printf("    %d. %s\n", xidmetler[i].kod, xidmetler[i].ad);
    }
    printf("\n");

    
    secim_gosterici = 0;
    while (!secim_gosterici) {
        printf("  Secimiz (1-%d): ", XIDMET_SAYI);
        if (scanf("%d", &secim) == 1 && secim >= 1 && secim <= XIDMET_SAYI) {
            secim_gosterici = 1; 
        } else {
            printf("  [XETA] Zehmet olmasa 1-%d arasinda secim edin.\n", XIDMET_SAYI);
        }
        buffer_temizle();
    }

    
    printf("  Musteri / abune kodunuzu daxil edin: ");
    if (fgets(musteri_kodu, MAX_KOD_UZUNLUGU, stdin) != NULL) {
       
        size_t uz = strlen(musteri_kodu);
        if (uz > 0 && musteri_kodu[uz - 1] == '\n')
            musteri_kodu[uz - 1] = '\0';
    }

    
    odenis_meblegi = double_oxu("  Odenis meblegi (AZN): ");

    
    if (odenis_meblegi <= 0.0) {
        printf("\n  [XETA] Odenis meblegi musbat olmalidir.\n");
        return;
    }

    
    if (odenis_meblegi < xidmetler[secim - 1].min_odenis) {
        printf("\n  [XETA] Bu xidmet ucun minimum odenis: %.2f AZN\n",
               xidmetler[secim - 1].min_odenis);
        return;
    }

   
    if (kart->balans < odenis_meblegi) {
        printf("\n  [XETA] Balans kifayet etmir!\n");
        printf("  Cari balans : %.2f AZN\n", kart->balans);
        printf("  Lazim olan  : %.2f AZN\n", odenis_meblegi);
        return;
    }

   
    kart->balans -= odenis_meblegi;

    
    xett_ciz();
    printf("          ODENİS QEBZİ\n");
    xett_ciz();
    printf("  Xidmet       : %s\n",    xidmetler[secim - 1].ad);
    printf("  Musteri kodu : %s\n",    musteri_kodu);
    printf("  Odenis       : %.2f AZN\n", odenis_meblegi);
    printf("  Yeni balans  : %.2f AZN\n", kart->balans);
    xett_ciz();
    printf("  ✓ Odenis ugurla tamamlandi!\n");
    xett_ciz();
}



void koc_pulu(Kart *gonderici, Kart *alici)
{
    char   alici_kart_no[KART_NOMRESI_UZUNLUGU]; 
    double kocurme_meblegi;                       

    xett_ciz();
    printf("         KART-DAN KARTA KOCURME\n");
    xett_ciz();

    
    kart_nomresi_oxu(alici_kart_no);

    

    
    if (strcmp(alici_kart_no, gonderici->kart_nomresi) == 0) {
        printf("\n  [XETA] Oz kartiniza kocurme ede bilmezsiniz.\n");
        return;
    }

    
    kocurme_meblegi = double_oxu("  Kocurme meblegi (AZN): ");

    
    if (kocurme_meblegi < MIN_KOCURME_MEBLEGI) {
        printf("\n  [XETA] Minimum kocurme meblegi: %.2f AZN\n", MIN_KOCURME_MEBLEGI);
        return;
    }
    if (kocurme_meblegi > MAX_KOCURME_MEBLEGI) {
        printf("\n  [XETA] Maksimum kocurme meblegi: %.2f AZN\n", MAX_KOCURME_MEBLEGI);
        return;
    }

    
    if (gonderici->balans < kocurme_meblegi) {
        printf("\n  [XETA] Balans kifayet etmir!\n");
        printf("  Cari balans   : %.2f AZN\n", gonderici->balans);
        printf("  Kocurme ucun  : %.2f AZN lazimdir\n", kocurme_meblegi);
        return;
    }

    
    gonderici->balans -= kocurme_meblegi;  
    alici->balans     += kocurme_meblegi;  

    
    xett_ciz();
    printf("         KOCURME QEBZİ\n");
    xett_ciz();
    printf("  Gonderici kart : %s\n",    gonderici->kart_nomresi);
    printf("  Alici kart     : %s\n",    alici_kart_no);
    printf("  Kocurulen      : %.2f AZN\n", kocurme_meblegi);
    printf("  Sizin yeni bal.: %.2f AZN\n", gonderici->balans);
    xett_ciz();
    printf("  ✓ Kocurme ugurla tamamlandi!\n");
    xett_ciz();
}



void cekil_naqd(Kart *kart)
{
    double cekilme_meblegi; 

    xett_ciz();
    printf("          NAĞDLAŞDIRMA (PUL CEKME)\n");
    xett_ciz();
    printf("  Cari balans : %.2f AZN\n", kart->balans);
    printf("  Limit       : %.2f - %.2f AZN\n",
           MIN_CEKILME_MEBLEGI, MAX_CEKILME_MEBLEGI);
    xett_ciz();

    
    cekilme_meblegi = double_oxu("  Cekilecek mebleg (AZN): ");

    
    if (cekilme_meblegi < MIN_CEKILME_MEBLEGI) {
        printf("\n  [XETA] Minimum cekilme meblegi %.2f AZN-dir.\n",
               MIN_CEKILME_MEBLEGI);
        return;
    }

    
    if (cekilme_meblegi > MAX_CEKILME_MEBLEGI) {
        printf("\n  [XETA] Maksimum cekilme meblegi %.2f AZN-dir.\n",
               MAX_CEKILME_MEBLEGI);
        return;
    }

    
    if (kart->balans < cekilme_meblegi) {
        printf("\n  [XETA] Balans kifayet etmir!\n");
        printf("  Cari balans  : %.2f AZN\n", kart->balans);
        printf("  Cekmek istey.: %.2f AZN\n", cekilme_meblegi);
        printf("  Catismazliq  : %.2f AZN\n",
               cekilme_meblegi - kart->balans);
        return;
    }

    
    kart->balans -= cekilme_meblegi;

    
    xett_ciz();
    printf("      NAĞDLAŞDIRMA QEBZİ\n");
    xett_ciz();
    printf("  Kart sahibi  : %s\n",    kart->sahib_adi);
    printf("  Verildi      : %.2f AZN\n", cekilme_meblegi);
    printf("  Yeni balans  : %.2f AZN\n", kart->balans);
    xett_ciz();
    printf("  ✓ Nağdlaşdirma ugurla tamamlandi!\n");
    xett_ciz();
}



void artir_balans(Kart *kart)
{
    double artirma_meblegi; 

    xett_ciz();
    printf("           BALANS ARTIRMA (MEDAXIL)\n");
    xett_ciz();
    printf("  Cari balans : %.2f AZN\n", kart->balans);
    xett_ciz();

    
    artirma_meblegi = double_oxu("  Elave edilecek mebleg (AZN): ");

    
    if (artirma_meblegi <= 0.0) {
        printf("\n  [XETA] Mebleg musbat olmalidir. Menfi ve ya sifir daxil etmek olmaz.\n");
        return;
    }

    
    if (artirma_meblegi > MAX_MEBLAQ_ARTIRMA) {
        printf("\n  [XETA] Bir dəfəlik maksimum artirma: %.2f AZN\n",
               MAX_MEBLAQ_ARTIRMA);
        return;
    }

    
    kart->balans += artirma_meblegi;

    
    xett_ciz();
    printf("          MEDAXIL QEBZİ\n");
    xett_ciz();
    printf("  Kart sahibi  : %s\n",    kart->sahib_adi);
    printf("  Elave edildi : %.2f AZN\n", artirma_meblegi);
    printf("  Yeni balans  : %.2f AZN\n", kart->balans);
    xett_ciz();
    printf("  ✓ Balans ugurla artırildi!\n");
    xett_ciz();
}




int main(void)
{
    
    Kart kart1 = {
        .kart_nomresi = "1234567890123456",
        .balans       = 500.00,
        .sahib_adi    = "Əli Həsənov"
    };

    Kart kart2 = {
        .kart_nomresi = "9876543210654321",
        .balans       = 100.00,
        .sahib_adi    = "Nigar Muradova"
    };

    int secim; 

    
    while (1) {
        printf("\n");
        xett_ciz();
        printf("         BANK SİSTEMİ - ANA MENYU\n");
        xett_ciz();
        printf("  Kart sahibi : %s\n", kart1.sahib_adi);
        printf("  Balans      : %.2f AZN\n\n", kart1.balans);
        printf("  1. Kommunal / Xidmet Odenisi\n");
        printf("  2. Kartdan Karta Kocurme\n");
        printf("  3. Nağdlaşdırma (Pul Cekme)\n");
        printf("  4. Balans Artirma (Medaxil)\n");
        printf("  0. Cixis\n");
        xett_ciz();
        printf("  Seciminiz: ");

        if (scanf("%d", &secim) != 1) {
            buffer_temizle();
            continue; 
        }
        buffer_temizle();

        switch (secim) {
            case 1:
                
                ode_kommunal(&kart1);
                break;

            case 2:
                
                koc_pulu(&kart1, &kart2);
                
                printf("  [TEST] %s-in yeni balansi: %.2f AZN\n",
                       kart2.sahib_adi, kart2.balans);
                break;

            case 3:
                
                cekil_naqd(&kart1);
                break;

            case 4:
                
                artir_balans(&kart1);
                break;

            case 0:
                
                printf("\n  Sistemden cixildi. Ugurlar!\n\n");
                return EXIT_SUCCESS;

            default:
                printf("\n  [XETA] Yalniz 0-4 arasinda secim edin.\n");
                break;
        }
    }

    return EXIT_SUCCESS;
}