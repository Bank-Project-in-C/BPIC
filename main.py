/**
 * ============================================================
 *  BANK SİSTEMİ - Əsas Maliyyə Əməliyyatları Modulu
 * ============================================================
 *  Müəllif   : Tələbə (Komanda Layihəsi)
 *  Dil       : C99 / C11
 *  Standart  : ISO C99 (-std=c99)
 *  Açıqlama  : Bu modul bank sisteminin dörd əsas əməliyyatını
 *              həyata keçirir: Ödəniş, Köçürmə, Nağdlaşdırma,
 *              Balans artırma.
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================
 *  SABİTLƏR (Magic Number-ların qarşısını almaq üçün #define)
 * ============================================================ */

/* Nağdlaşdırma limitləri (AZN) */
#define MIN_CEKILME_MEBLEGI     1.0
#define MAX_CEKILME_MEBLEGI  1000.0

/* Köçürmə limitləri (AZN) */
#define MIN_KOCURME_MEBLEGI     0.01
#define MAX_KOCURME_MEBLEGI  5000.0

/* Balans artırma limiti (AZN) */
#define MAX_MEBLAQ_ARTIRMA   10000.0

/* Kart nömrəsinin uzunluğu (16 rəqəm + '\0') */
#define KART_NOMRESI_UZUNLUGU  17

/* Müştəri kodunun maksimum uzunluğu */
#define MAX_KOD_UZUNLUGU       20

/* Xidmət növlərinin sayı */
#define XIDMET_SAYI             5

/* ============================================================
 *  VERİ STRUKTURLARI (Data Structures)
 * ============================================================ */

/**
 * Kart strukturu: Hər istifadəçinin bank kartı məlumatlarını saxlayır.
 * Pointer vasitəsilə ötürülür ki, funksiyalar dəyəri birbaşa dəyişə bilsin.
 */
typedef struct {
    char   kart_nomresi[KART_NOMRESI_UZUNLUGU]; /* 16 rəqəmli kart nömrəsi */
    double balans;                               /* Cari balans (AZN)        */
    char   sahib_adi[50];                        /* Kart sahibinin adı       */
} Kart;

/**
 * Xidmət strukturu: Kommunal ödəniş üçün xidmət növünü təmsil edir.
 */
typedef struct {
    int  kod;         /* Xidmətin seçim nömrəsi */
    char ad[30];      /* Xidmətin adı            */
    double min_odenis; /* Minimum ödəniş məbləği  */
} Xidmet;

/* ============================================================
 *  YARDIMÇI (HELPER) FUNKSİYALAR
 * ============================================================ */

/**
 * buffer_temizle:
 *  scanf sonrasında stdin-də qalan '\n' və digər
 *  simvolları təmizləyir. Bu olmadan növbəti fgets/scanf
 *  yanlış işləyə bilər.
 */
static void buffer_temizle(void)
{
    int c;
    /* Sona qədər bütün simvolları oxu və at */
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

/**
 * xett_ciz:
 *  Ekrana dekorativ ayırıcı xətt çəkir (qəbz üçün).
 */
static void xett_ciz(void)
{
    printf("--------------------------------------------------\n");
}

/**
 * double_oxu:
 *  İstifadəçidən təhlükəsiz şəkildə `double` dəyər oxuyur.
 *  Yanlış giriş zamanı yenidən soruşur.
 *
 *  @param mesaj  : Ekranda göstəriləcək prompt mətni
 *  @return       : İstifadəçinin daxil etdiyi düzgün double dəyər
 */
static double double_oxu(const char *mesaj)
{
    double deger;
    int oxuma_sayi;

    while (1) {
        printf("%s", mesaj);
        oxuma_sayi = scanf("%lf", &deger);
        buffer_temizle(); /* Buffer-i mütləq təmizlə */

        if (oxuma_sayi == 1) {
            /* Uğurlu oxuma - dəyəri qaytar */
            return deger;
        }
        /* Yanlış giriş: istifadəçini xəbərdar et və yenidən soruş */
        printf("  [XETA] Zehmet olmasa düzgün rəqəm daxil edin.\n");
    }
}

/**
 * kart_nomresi_oxu:
 *  16 rəqəmli kart nömrəsini oxuyur və uzunluğunu yoxlayır.
 *
 *  @param hedef  : Kart nömrəsinin yazılacağı char massivi
 */
static void kart_nomresi_oxu(char *hedef)
{
    /* Giriş üçün əlavə boşluqlu müvəqqəti bufer: 16 rəqəm + '\n' + '\0' = 18 */
    char temp_buf[KART_NOMRESI_UZUNLUGU + 1]; /* 17 + 1 = 18 simvol */

    while (1) {
        printf("  Kart nomresi (16 reqem): ");
        /* fgets ilə buffer daşmasının qarşısını al — temp_buf-a oxu */
        if (fgets(temp_buf, sizeof(temp_buf), stdin) != NULL) {
            /* temp_buf-dakı '\n'-i sil, uzunluğu hesabla */
            size_t uzunlug = strlen(temp_buf);
            if (uzunlug > 0 && temp_buf[uzunlug - 1] == '\n') {
                temp_buf[uzunlug - 1] = '\0';
                uzunlug--;
            } else {
                /* '\n' yoxdursa giriş 16-dan uzundur — artığını təmizlə */
                buffer_temizle();
            }

            /* Uzunluq yoxlaması: tam 16 rəqəm olmalıdır */
            if (uzunlug == 16) {
                /* Düzgün format — hedef massivə kopyala və çıx */
                strncpy(hedef, temp_buf, KART_NOMRESI_UZUNLUGU);
                hedef[KART_NOMRESI_UZUNLUGU - 1] = '\0'; /* Zəmanətli '\0' */
                return;
            }
            printf("  [XETA] Kart nomresi tam 16 reqemden ibaret olmalidir.\n");
        }
    }
}

/* ============================================================
 *  ANA FUNKSİYALAR
 * ============================================================ */

/**
 * ============================================================
 *  1. ODE_KOMMUNAL — Kommunal/Xidmət Ödənişi
 * ============================================================
 *
 *  Məntiq:
 *    - Mövcud xidmət siyahısını göstər
 *    - İstifadəçidən xidmət seçimi, müştəri kodu və məbləği al
 *    - Balansı yoxla: kifayətsə çıx, deyilsə xəta ver
 *    - Uğurlu ödəniş üçün ekrana qəbz çap et
 *
 *  @param kart  : Cari istifadəçinin kartına pointer (balans dəyişəcək)
 */
void ode_kommunal(Kart *kart)
{
    /* Mövcud xidmətlər siyahısı — genişləndirilə bilər */
    const Xidmet xidmetler[XIDMET_SAYI] = {
        {1, "Ishiq (Azerenerji)",   1.00},
        {2, "Qaz (Azerigas)",       1.00},
        {3, "Su (Azersu)",          0.50},
        {4, "Internet / TV",        5.00},
        {5, "Mobil telefon",        1.00}
    };

    int     secim;           /* İstifadəçinin xidmət seçimi */
    char    musteri_kodu[MAX_KOD_UZUNLUGU]; /* Müştəri / abunə kodu */
    double  odenis_meblegi;  /* Ödəniləcək məbləğ         */
    int     secim_gosterici; /* Seçimin etibarlı olub-olmadığı */

    xett_ciz();
    printf("       KOMMUNAL / XİDMƏT ODENİSİ\n");
    xett_ciz();

    /* Xidmət siyahısını ekrana çıxar */
    printf("  Xidmet seçin:\n");
    for (int i = 0; i < XIDMET_SAYI; i++) {
        printf("    %d. %s\n", xidmetler[i].kod, xidmetler[i].ad);
    }
    printf("\n");

    /* Xidmət seçimini oxu və etibarlılığını yoxla */
    secim_gosterici = 0;
    while (!secim_gosterici) {
        printf("  Secimiz (1-%d): ", XIDMET_SAYI);
        if (scanf("%d", &secim) == 1 && secim >= 1 && secim <= XIDMET_SAYI) {
            secim_gosterici = 1; /* Düzgün seçim */
        } else {
            printf("  [XETA] Zehmet olmasa 1-%d arasinda secim edin.\n", XIDMET_SAYI);
        }
        buffer_temizle();
    }

    /* Müştəri / abunəçi kodunu oxu */
    printf("  Musteri / abune kodunuzu daxil edin: ");
    if (fgets(musteri_kodu, MAX_KOD_UZUNLUGU, stdin) != NULL) {
        /* Sondakı '\n'-i sil */
        size_t uz = strlen(musteri_kodu);
        if (uz > 0 && musteri_kodu[uz - 1] == '\n')
            musteri_kodu[uz - 1] = '\0';
    }

    /* Ödəniş məbləğini al */
    odenis_meblegi = double_oxu("  Odenis meblegi (AZN): ");

    /* Mənfi məbləğin qarşısını al */
    if (odenis_meblegi <= 0.0) {
        printf("\n  [XETA] Odenis meblegi musbat olmalidir.\n");
        return;
    }

    /* Xidmətin minimum məbləğini yoxla */
    if (odenis_meblegi < xidmetler[secim - 1].min_odenis) {
        printf("\n  [XETA] Bu xidmet ucun minimum odenis: %.2f AZN\n",
               xidmetler[secim - 1].min_odenis);
        return;
    }

    /* Balans yetərliliyini yoxla */
    if (kart->balans < odenis_meblegi) {
        printf("\n  [XETA] Balans kifayet etmir!\n");
        printf("  Cari balans : %.2f AZN\n", kart->balans);
        printf("  Lazim olan  : %.2f AZN\n", odenis_meblegi);
        return;
    }

    /* ✅ Balansdan məbləği çıx — pointer vasitəsilə real dəyişiklik */
    kart->balans -= odenis_meblegi;

    /* Uğurlu ödəniş qəbzini çap et */
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


/**
 * ============================================================
 *  2. KOC_PULU — Kartdan Karta / Hesaba Köçürmə
 * ============================================================
 *
 *  Məntiq:
 *    - Alıcı kart nömrəsini al (16 rəqəm)
 *    - Köçürülən məbləği al
 *    - Gönərənin balansını yoxla
 *    - Pointer-lər vasitəsilə: göndərəndən çıx, alıcıya əlavə et
 *
 *  @param gonderici  : Pulu göndərən kartın pointeri
 *  @param alici      : Pulu alan kartın pointeri
 *                      (real sistemdə DB-dən tapılır; burada test üçün ötürülür)
 */
void koc_pulu(Kart *gonderici, Kart *alici)
{
    char   alici_kart_no[KART_NOMRESI_UZUNLUGU]; /* Daxil edilən alıcı kart nömrəsi */
    double kocurme_meblegi;                       /* Köçürüləcək məbləğ              */

    xett_ciz();
    printf("         KART-DAN KARTA KOCURME\n");
    xett_ciz();

    /* Alıcının kart nömrəsini oxu */
    kart_nomresi_oxu(alici_kart_no);

    /**
     * NOT: Real sistemdə bu nöqtədə verilənlər bazasında
     * 'alici_kart_no' ilə uyğun kart axtarılır.
     * Biz test üçün 'alici' pointerini birbaşa ötürürük.
     */

    /* Öz kartına köçürməni qadağan et */
    if (strcmp(alici_kart_no, gonderici->kart_nomresi) == 0) {
        printf("\n  [XETA] Oz kartiniza kocurme ede bilmezsiniz.\n");
        return;
    }

    /* Köçürmə məbləğini al */
    kocurme_meblegi = double_oxu("  Kocurme meblegi (AZN): ");

    /* Məbləğ limitleri yoxlaması */
    if (kocurme_meblegi < MIN_KOCURME_MEBLEGI) {
        printf("\n  [XETA] Minimum kocurme meblegi: %.2f AZN\n", MIN_KOCURME_MEBLEGI);
        return;
    }
    if (kocurme_meblegi > MAX_KOCURME_MEBLEGI) {
        printf("\n  [XETA] Maksimum kocurme meblegi: %.2f AZN\n", MAX_KOCURME_MEBLEGI);
        return;
    }

    /* Göndərənin balansının yetərliliyini yoxla */
    if (gonderici->balans < kocurme_meblegi) {
        printf("\n  [XETA] Balans kifayet etmir!\n");
        printf("  Cari balans   : %.2f AZN\n", gonderici->balans);
        printf("  Kocurme ucun  : %.2f AZN lazimdir\n", kocurme_meblegi);
        return;
    }

    /* ✅ Pointer vasitəsilə hər iki balansı dəyişdir */
    gonderici->balans -= kocurme_meblegi;  /* Göndərəndən çıx  */
    alici->balans     += kocurme_meblegi;  /* Alıcıya əlavə et */

    /* Köçürmə qəbzini çap et */
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


/**
 * ============================================================
 *  3. CEKIL_NAQD — Nağdlaşdırma (Balansdan Pul Çıxarma)
 * ============================================================
 *
 *  Məntiq:
 *    - İstifadəçidən çəkiləcək məbləği al
 *    - Min/Max limit yoxlaması et
 *    - Balans yoxlaması et
 *    - Pointer vasitəsilə balansı azalt
 *
 *  @param kart  : Cari istifadəçinin kartına pointer
 */
void cekil_naqd(Kart *kart)
{
    double cekilme_meblegi; /* Çəkiləcək məbləğ */

    xett_ciz();
    printf("          NAĞDLAŞDIRMA (PUL CEKME)\n");
    xett_ciz();
    printf("  Cari balans : %.2f AZN\n", kart->balans);
    printf("  Limit       : %.2f - %.2f AZN\n",
           MIN_CEKILME_MEBLEGI, MAX_CEKILME_MEBLEGI);
    xett_ciz();

    /* Məbləği istifadəçidən al */
    cekilme_meblegi = double_oxu("  Cekilecek mebleg (AZN): ");

    /* Minimum limit yoxlaması */
    if (cekilme_meblegi < MIN_CEKILME_MEBLEGI) {
        printf("\n  [XETA] Minimum cekilme meblegi %.2f AZN-dir.\n",
               MIN_CEKILME_MEBLEGI);
        return;
    }

    /* Maksimum limit yoxlaması */
    if (cekilme_meblegi > MAX_CEKILME_MEBLEGI) {
        printf("\n  [XETA] Maksimum cekilme meblegi %.2f AZN-dir.\n",
               MAX_CEKILME_MEBLEGI);
        return;
    }

    /* Balans yetərliliyini yoxla */
    if (kart->balans < cekilme_meblegi) {
        printf("\n  [XETA] Balans kifayet etmir!\n");
        printf("  Cari balans  : %.2f AZN\n", kart->balans);
        printf("  Cekmek istey.: %.2f AZN\n", cekilme_meblegi);
        printf("  Catismazliq  : %.2f AZN\n",
               cekilme_meblegi - kart->balans);
        return;
    }

    /* ✅ Balansdan məbləği çıx */
    kart->balans -= cekilme_meblegi;

    /* Nağdlaşdırma qəbzini çap et */
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


/**
 * ============================================================
 *  4. ARTIR_BALANS — Balansa Pul Əlavə Etmə (Mədaxil)
 * ============================================================
 *
 *  Məntiq:
 *    - İstifadəçidən əlavə ediləcək məbləği al
 *    - Mənfi və ya sıfır məbləğin qarşısını al
 *    - Maksimum həddini yoxla
 *    - Pointer vasitəsilə balansı artır
 *
 *  @param kart  : Cari istifadəçinin kartına pointer
 */
void artir_balans(Kart *kart)
{
    double artirma_meblegi; /* Əlavə ediləcək məbləğ */

    xett_ciz();
    printf("           BALANS ARTIRMA (MEDAXIL)\n");
    xett_ciz();
    printf("  Cari balans : %.2f AZN\n", kart->balans);
    xett_ciz();

    /* Məbləği istifadəçidən al */
    artirma_meblegi = double_oxu("  Elave edilecek mebleg (AZN): ");

    /* Mənfi və ya sıfır məbləğin qarşısını al */
    if (artirma_meblegi <= 0.0) {
        printf("\n  [XETA] Mebleg musbat olmalidir. Menfi ve ya sifir daxil etmek olmaz.\n");
        return;
    }

    /* Maksimum limit yoxlaması (bir dəfəlik artırma üçün) */
    if (artirma_meblegi > MAX_MEBLAQ_ARTIRMA) {
        printf("\n  [XETA] Bir dəfəlik maksimum artirma: %.2f AZN\n",
               MAX_MEBLAQ_ARTIRMA);
        return;
    }

    /* ✅ Balansı artır — pointer vasitəsilə əsl dəyişiklik */
    kart->balans += artirma_meblegi;

    /* Mədaxil qəbzini çap et */
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


/* ============================================================
 *  TEST ÜÇÜN main() — Komanda yoldaşlarına inteqrasiya nümunəsi
 * ============================================================ */

int main(void)
{
    /**
     * Test kartları:
     *  kart1 — cari istifadəçi (gönderici)
     *  kart2 — köçürmə zamanı alıcı rol oynayır
     */
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

    int secim; /* Menyudan istifadəçinin seçimi */

    /* Ana menyu döngüsü */
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
            continue; /* Yanlış giriş — yenidən menyu göstər */
        }
        buffer_temizle();

        switch (secim) {
            case 1:
                /* Kommunal ödəniş funksiyasını çağır */
                ode_kommunal(&kart1);
                break;

            case 2:
                /**
                 * Köçürmə: real sistemdə alıcı kart DB-dən tapılır.
                 * Burada test üçün kart2-ni alıcı kimi ötürürük.
                 */
                koc_pulu(&kart1, &kart2);
                /* Test üçün alıcının yeni balansını göstər */
                printf("  [TEST] %s-in yeni balansi: %.2f AZN\n",
                       kart2.sahib_adi, kart2.balans);
                break;

            case 3:
                /* Nağdlaşdırma funksiyasını çağır */
                cekil_naqd(&kart1);
                break;

            case 4:
                /* Balans artırma funksiyasını çağır */
                artir_balans(&kart1);
                break;

            case 0:
                /* Proqramdan çıxış */
                printf("\n  Sistemden cixildi. Ugurlar!\n\n");
                return EXIT_SUCCESS;

            default:
                printf("\n  [XETA] Yalniz 0-4 arasinda secim edin.\n");
                break;
        }
    }

    return EXIT_SUCCESS;
}