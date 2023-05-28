#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Nokta akoordinatlarını tutar
typedef struct {
    float x;
    float y;
} VeriNoktasi;

//iki nokta arasındaki Öklidyen mesafesini hesaplar
float oklidyenMesafesi(VeriNoktasi p1, VeriNoktasi p2) {
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    return sqrt(dx*dx + dy*dy);
}

void meanShiftKumeleri(VeriNoktasi* veriNoktalari, int veriBoyutu, float bantGenisligi, float yakinsamaEsiği) {
    // Yoğunluk merkezlerini hesaplamak için başlangıç noktalarını veri noktalarıyla eşleştir
    VeriNoktasi* yoğunlukMerkezleri = (VeriNoktasi*)malloc(veriBoyutu * sizeof(VeriNoktasi));
    for (int i = 0; i < veriBoyutu; i++) {
        yoğunlukMerkezleri[i] = veriNoktalari[i];
    }

    // Yoğunluk merkezlerini güncelle
    int iterasyonSayisi = 0;
    int degisiklikSayisi;

    do {
        degisiklikSayisi = 0;

        for (int i = 0; i < veriBoyutu; i++) {
            VeriNoktasi merkez = yoğunlukMerkezleri[i];
            VeriNoktasi yeniMerkez = merkez;

            int bantGenisligiIcindekiNoktaSayisi = 0;
            float xToplam = 0;
            float yToplam = 0;

            for (int j = 0; j < veriBoyutu; j++) {
                float mesafe = oklidyenMesafesi(veriNoktalari[j], merkez);

                if (mesafe <= bantGenisligi) {
                    xToplam += veriNoktalari[j].x;
                    yToplam += veriNoktalari[j].y;
                    bantGenisligiIcindekiNoktaSayisi++;
                }
            }

            if (bantGenisligiIcindekiNoktaSayisi > 0) {
                yeniMerkez.x = xToplam / bantGenisligiIcindekiNoktaSayisi;
                yeniMerkez.y = yToplam / bantGenisligiIcindekiNoktaSayisi;
            }

            if (oklidyenMesafesi(merkez, yeniMerkez) > yakinsamaEsiği) {
                yoğunlukMerkezleri[i] = yeniMerkez;
                degisiklikSayisi++;
            }
        }

        iterasyonSayisi++;
    } while (degisiklikSayisi > 0);

    // Küme etiketlerini belirle
    int* kumeEtiketleri = (int*)malloc(veriBoyutu * sizeof(int));
    int kumeIndeksi = 0;

    for (int i = 0; i < veriBoyutu; i++) {
        kumeEtiketleri[i] = -1;
    }

    for (int i = 0; i < veriBoyutu; i++) {
        if (kumeEtiketleri[i] == -1) {
            kumeEtiketleri[i] = kumeIndeksi;

            for (int j = i + 1; j < veriBoyutu; j++) {
                if (oklidyenMesafesi(yoğunlukMerkezleri[i], veriNoktalari[j]) <= bantGenisligi) {
                    kumeEtiketleri[j] = kumeIndeksi;
                }
            }

            kumeIndeksi++;
        }
    }

    // Sonuçları yazdır
    for (int i = 0; i < veriBoyutu; i++) {
        printf("Nokta %d -> X:%.2f, Y:%.2f, sinif:%d\n", i+1, veriNoktalari[i].x, veriNoktalari[i].y, kumeEtiketleri[i]);
    }
    // dizilerinin belleklerini serbest bırakır
    free(yoğunlukMerkezleri);
    free(kumeEtiketleri);
}

int main() {
    //Atama
    int veriBoyutu = 10;
    float bantGenisligi = 1.0;
    float yakinsamaEsiği = 0.001;

    //Bellek bloklarını ayırır
    VeriNoktasi* veriNoktalari = (VeriNoktasi*)malloc(veriBoyutu * sizeof(VeriNoktasi));
    if (veriNoktalari == NULL) {
        printf("Bellek hatası!");
        return 1;
    }

    // Veri noktalarını doldur (manuel)
    veriNoktalari[0].x = 2.0; veriNoktalari[0].y = 3.0;
    veriNoktalari[1].x = 2.5; veriNoktalari[1].y = 3.5;
    veriNoktalari[2].x = 1.5; veriNoktalari[2].y = 2.5;
    veriNoktalari[3].x = 5.0; veriNoktalari[3].y = 6.0;
    veriNoktalari[4].x = 6.0; veriNoktalari[4].y = 7.0;
    veriNoktalari[5].x = 5.5; veriNoktalari[5].y = 6.5;
    veriNoktalari[6].x = 9.0; veriNoktalari[6].y = 8.0;
    veriNoktalari[7].x = 8.5; veriNoktalari[7].y = 7.5;
    veriNoktalari[8].x = 9.5; veriNoktalari[8].y = 8.5;
    veriNoktalari[9].x = 8.0; veriNoktalari[9].y = 7.0;

    meanShiftKumeleri(veriNoktalari, veriBoyutu, bantGenisligi, yakinsamaEsiği);

    free(veriNoktalari);

    return 0;
}
