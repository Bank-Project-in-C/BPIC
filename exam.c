#include <stdio.h>

typedef struct {
    char isim[50];
    int yas;
    int dogum_yili;
} Ogretmen;

int main() {
    int n;
    // Kaç tane öğretmen girileceğini alıyoruz (Örn: 3)
    scanf("%d", &n);

    // Öğretmen yapısından N boyutunda bir array oluşturuyoruz!
    // Kodu uzatmayı engelleyen sihirli kısım tam olarak burası.
    Ogretmen ogretmenler[n];

    // 1. Bilgileri Inputtan Okuma
    for (int i = 0; i < n; i++) {
        // struct elemanlarına NOKTA (.) işareti ile erişilir: ogretmenler[i].isim gibi
        scanf("%s %d %d", ogretmenler[i].isim, &ogretmenler[i].yas, &ogretmenler[i].dogum_yili);
    }

    printf("\n--- Kaydedilen Ogretmenler ---\n");

    // 2. Bilgileri Ekrana Yazdırma
    for (int i = 0; i < n; i++) {
        printf("%d. Ogretmen -> Isim: %s, Yas: %d, Dogum Yili: %d\n", 
               i + 1, ogretmenler[i].isim, ogretmenler[i].yas, ogretmenler[i].dogum_yili);
    }

    return 0;
}