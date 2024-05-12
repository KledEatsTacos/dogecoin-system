#include <iostream>
#include <ctime>
#include <iomanip>
#include <cstdlib>
using namespace std;

const int maksimumHesaplar = 100; //max hesap sayısı 100 olsun

class Hesap {
private:
	int hesapNo;
	double miktar;
	double dogecoin = 0; //dogecoin adlı bir kripto para birimi
	bool GenelHesap; //iki hesap türü var, genel ve özel hesaplar

	int sonGuncellenenGun; //son güncellemeden bu yana geçen gün sayısı
	double dogecoinDegeri;

	int sonDegisiklikGun;
	int sonDegisiklikAy;
	int sonDegisiklikYil;

	bool ilkAlimYapildi = false; //ilk alım yapıldı mı kontrolü

public:
	Hesap() {}
	Hesap(int hesapNo, double miktar, bool genelHesap) {
		this->hesapNo = hesapNo;
		this->miktar = miktar;
		this->GenelHesap = genelHesap;
	}

	void sonDegisiklikTarihiniAyarla(int gun, int ay, int yil) { //son değişiklik tarihini ayarlayan fonksiyon
		sonDegisiklikGun = gun;
		sonDegisiklikAy = ay;
		sonDegisiklikYil = yil;
	}

	//Aydaki gün sayısını hesaplayan fonksiyon
	int aydakiGunSayisi(int ay, int yil) {
		if (ay == 2) {
			//Şubat ayı - Artık yıl kontrolü
			if ((yil % 4 == 0 && yil % 100 != 0) || (yil % 400 == 0)) {
				return 29; //Artık yıl, 29 gün
			}
			return 28; //Normal yıl, 28 gün
		}
		else if (ay == 4 || ay == 6 || ay == 9 || ay == 11) {
			return 30; //30 gün olan aylar
		}
		return 31; //Diğer aylar, 31 gün
	}

	int sonDegisikliktenGecenGunleriHesapla(int suankiGun, int suankiAy, int suankiYil) {
		int gecenGunSayisi = 0;

		while (!(suankiYil == sonDegisiklikYil && suankiAy == sonDegisiklikAy && suankiGun == sonDegisiklikGun)) {
			// Aydaki gün sayısını al
			int aydakiGunler = aydakiGunSayisi(sonDegisiklikAy, sonDegisiklikYil);

			gecenGunSayisi++; // Gün sayısını artır

			// Sonraki günü ayarla
			sonDegisiklikGun++;

			if (sonDegisiklikGun > aydakiGunler) {
				// Sonraki aya geç
				sonDegisiklikGun = 1;
				sonDegisiklikAy++;

				if (sonDegisiklikAy > 12) {
					// Sonraki yıla geç
					sonDegisiklikAy = 1;
					sonDegisiklikYil++;
				}
			}


			suankiGun = sonDegisiklikGun;
			suankiAy = sonDegisiklikAy;
			suankiYil = sonDegisiklikYil;
		}

		return gecenGunSayisi;
	}

	void genelHesapGuncelle(int suankiGun) {
		int gecenGunler = suankiGun - sonGuncellenenGun;
		double artisYuzdesi = 0.01;
		double azalisYuzdesi = 0.05;

		if (gecenGunler >= 15) {
			int guncellemeAdedi = gecenGunler / 15;
			for (int i = 0; i < guncellemeAdedi; ++i) {
				dogecoinDegeri -= (dogecoinDegeri * azalisYuzdesi);
				sonGuncellenenGun += 15;
			}
		}
		else {
			dogecoinDegeri += (dogecoinDegeri * artisYuzdesi * gecenGunler);
			sonGuncellenenGun = suankiGun;
		}
	}


	void ozelHesapGuncelle(int suankiGun) {
		int gecenGunler = suankiGun - sonGuncellenenGun;
		double artisYuzdesi = 0.05;
		double azalisYuzdesi = 0.1;
		int rastgeleGunler[3] = { 5, 15, 25 }; // Random days for value drop

		if (gecenGunler >= 30) {
			int guncellemeAdedi = gecenGunler / 30;
			for (int i = 0; i < guncellemeAdedi; ++i) {
				for (int j = 0; j < 3; ++j) {
					if (sonGuncellenenGun + 30 == rastgeleGunler[j]) {
						dogecoinDegeri -= (dogecoinDegeri * azalisYuzdesi);
						sonGuncellenenGun += 30;
						break;
					}
				}
			}
		}
		else {
			dogecoinDegeri += (dogecoinDegeri * artisYuzdesi * gecenGunler);
			sonGuncellenenGun = suankiGun;
		}
	}

	void ilkAlim(double yatirilanPara) {
		if (dogecoin == 0) {
			// Calculate the amount of Dogecoin to be bought based on the initial purchase
			dogecoin = yatirilanPara; // Initial purchase sets the Dogecoin value to 1 TL per coin
			miktar -= yatirilanPara; // Deduct the invested money from the balance
			cout << yatirilanPara << " TL karşılığı " << dogecoin << " Dogecoin " << hesapNo << " numaralı hesaba eklendi." << endl;
		}
		else {
			// Logic for subsequent purchases
			double additionalDogecoin = yatirilanPara; // Modify this line to determine additional Dogecoin purchased
			dogecoin += additionalDogecoin; // Add the additional Dogecoin to the existing balance
			miktar -= yatirilanPara; // Deduct the invested money from the balance
			cout << yatirilanPara << " TL karşılığı " << additionalDogecoin << " Dogecoin " << hesapNo << " numaralı hesaba eklendi." << endl;
		}
	}


	bool hesapMevcutMu(Hesap hesaplar[], int hesapSayisi, int hesapNo) { //aynı hesap numarasıyla birden fazla hesap açılmaması için
		for (int i = 0; i < hesapSayisi; i++) {
			if (hesaplar[i].hesapNo == hesapNo) {
				return true;
			}
		}
		return false;
	}

	bool hesapTuruSorgula() {
		return GenelHesap;
	}

	int hesapNumarasiniSorgula() {
		return hesapNo;
	}

	double dogecoinBakiyeSorgula() {
		return dogecoin;
	}

	double miktarSorgula() {
		return miktar;
	}

	void dogecoinDegeriBelirle(double deger) {
		dogecoin = deger;
	}

	void hesapListesineEkle(Hesap hesaplar[], int& hesapSayisi) { //hesap ekleme ve kaydetme işlemleri
		hesaplar[hesapSayisi] = *this;
		hesapSayisi++;
	}

	void listele() { //hesap listeleme işlemi
		cout << "Hesap numarasi: " << this->hesapNo << endl;
		cout << "Bakiye: " << this->miktar << endl;
		cout << "Dogecoin: " << this->dogecoin << endl;
		cout << "Hesap turu: " << (this->GenelHesap ? "Genel" : "Ozel") << endl;
	}

	void paraYatir(double miktar) { //para yatırma ve çekme işlemleri
		this->miktar += miktar;
	}
	void paraCek(double miktar) {
		this->miktar -= miktar;
	}

	void bakiyeSorgula() { //bakiye sorgulama
		cout << "Bakiyeniz: " << this->miktar << endl;
	}

	double kar(int suankiGun, int suankiAy, int suankiYil) {
		int gecenGunler = sonDegisikliktenGecenGunleriHesapla(suankiGun, suankiAy, suankiYil);


		double initialDogecoinValue = 0;
		if (ilkAlimYapildi) {
			initialDogecoinValue = dogecoin / (1 + (GenelHesap ? 0.01 : 0.05) * gecenGunler);
		}

		// Calculate the current Dogecoin value
		double currentDogecoinValue = dogecoinDegeri;

		// Calculate the profit or loss
		double profit = currentDogecoinValue - initialDogecoinValue;

		return profit;

	};
};

void suankiTarihiAl(int& gun, int& ay, int& yil) {
	time_t simdiki = time(0); // Get the current system time
	tm yerelZaman; // Create a tm struct for the local time

	if (localtime_s(&yerelZaman, &simdiki) == 0) { // Retrieve the local time
		// Set gun, month, and year based on the current system date
		gun = yerelZaman.tm_mday;
		ay = yerelZaman.tm_mon + 1; // Months are zero-indexed, so increment by 1
		yil = yerelZaman.tm_year + 1900; // Years since 1900
	}
	else {
		// Handle error if unable to retrieve local time
		cout << "Hata: Gecerli tarih alinmadi" << endl;
	}
}

void tarihiIlerlet(int& gun, int& ay, int& yil, int gunler) {
	const int aydakiGunler[] = { 31, 28, 31, 30, 31, 30, 31, 31,
			30, 31, 30, 31 }; // Days in each month

	gun += gun; // Increment day by the specified number of days

	// Loop to handle carry-over to the next month/year
	while (gun > aydakiGunler[ay - 1]) {
		gun -= aydakiGunler[ay - 1]; // Adjust the day

		if (++ay > 12) { // Move to the next year if necessary
			ay = 1; // Reset month to January
			++yil; // Increment year
		}
	}
}


int main()
{
	srand(time(0));
	setlocale(LC_ALL, "Turkish");//Türkçe kullanmak için
	int gun, ay, yil;
	suankiTarihiAl(gun, ay, yil);

	Hesap hesaplar[maksimumHesaplar];
	int hesapSayisi = 0;
	int secim;

	do {
		cout << "Ne yapacağız?: " << endl;
		cout << "\t 1. Genel hesap aç" << endl;
		cout << "\t 2. Özel hesap aç" << endl;
		cout << "\t 3. Hesapları listele" << endl;
		cout << "\t 4. Hesaba para yatır" << endl;
		cout << "\t 5. Hesaptan para çek" << endl;
		cout << "\t 6. Dogecoin al" << endl;
		cout << "\t 7. Dogecoin sat" << endl;
		cout << "\t 8. Tarih" << endl;
		cout << "\t 9. Zaman ilerle" << endl;
		cout << "\t 10. Kar/zarar hesapla" << endl;
		cout << "\t 11. Programdan çık" << endl;
		cin >> secim;
		switch (secim) {
		case 1: {
			int hesapNo;
			double miktar;
			cout << "Hesap numarasını giriniz: "; cin >> hesapNo;
			cout << "Miktari giriniz: "; cin >> miktar;

			if (hesaplar[0].hesapMevcutMu(hesaplar, hesapSayisi, hesapNo))
				cout << "Bu numaranın altında zaten bir hesap var!" << endl;

			else {
				Hesap yeniGenel(hesapNo, miktar, true);
				yeniGenel.hesapListesineEkle(hesaplar, hesapSayisi);
			}
			break;
		}
		case 2: {
			int hesapNo;
			double miktar;
			cout << "Hesap numarasını giriniz: "; cin >> hesapNo;
			cout << "Miktarini giriniz: "; cin >> miktar;

			if (hesaplar[0].hesapMevcutMu(hesaplar, hesapSayisi, hesapNo))
				cout << "Bu numaranın altında zaten bir hesap var!: " << endl;

			else {
				Hesap yeniOzel(hesapNo, miktar, false);
				yeniOzel.hesapListesineEkle(hesaplar, hesapSayisi);
			}
			break;
		}
		case 3: {
			if (hesapSayisi == 0)
				cout << "Hesap bulunamadı!" << endl;
			else {
				cout << "Hesaplar: " << endl;
				for (int i = 0; i < hesapSayisi; i++) {
					hesaplar[i].listele();
					cout << endl;
				}
			}
			break;
		}
		case 4: {
			int hesapNo;
			double miktar;
			bool hesapBulundu = false; //hesap bulundu mu kontrolü

			cout << "Hesap numarasını giriniz: "; cin >> hesapNo;
			cout << "Ne kadar para eklemek istediğinizi giriniz: "; cin >> miktar;
			for (int i = 0; i < hesapSayisi; i++) {
				if (hesaplar[i].hesapNumarasiniSorgula() == hesapNo) {
					hesaplar[i].paraYatir(miktar);
					cout << miktar << "TL " << hesapNo << " numaralı hesaba yattırıldı." << endl;
					hesapBulundu = true;
					break;
				}
			}
			if (!hesapBulundu) cout << "Hesap bulunamadı!" << endl;
			break;
		}
		case 5: {
			int hesapNumarasi;
			double miktar;
			bool hesapBulundu = false;
			cout << "Hesap numarasını giriniz: "; cin >> hesapNumarasi;
			cout << "Ne kadar para çekmek istediğinizi giriniz: "; cin >> miktar;
			for (int i = 0; i < hesapSayisi; i++) {
				if (hesaplar[i].hesapNumarasiniSorgula() == hesapNumarasi) {
					hesaplar[i].paraCek(miktar);
					cout << miktar << "TL " << hesapNumarasi << " numaralı hesaptan çekildi." << endl;
					hesapBulundu = true;
					break;
				}
			}
			if (!hesapBulundu) cout << "Hesap bulunamadı!" << endl;
			break;
		}
		case 6: {
			int hesapNo;
			double yatirilanPara;
			bool hesapBulundu = false;

			cout << "Hesap numarasını giriniz: "; cin >> hesapNo;
			cout << "Ne kadar para yatırmak istediğinizi giriniz: "; cin >> yatirilanPara;

			for (int i = 0; i < hesapSayisi; i++) {
				if (hesaplar[i].hesapNumarasiniSorgula() == hesapNo) {
					int gun, ay, yil;
					suankiTarihiAl(gun, ay, yil);


					bool isGenelHesap = hesaplar[i].hesapTuruSorgula();


					if (isGenelHesap) {
						hesaplar[i].genelHesapGuncelle(gun);
					}
					else {
						hesaplar[i].ozelHesapGuncelle(gun);
					}


					hesaplar[i].ilkAlim(yatirilanPara);

					hesapBulundu = true;
					break;
				}
			}

			if (!hesapBulundu) cout << "Hesap bulunamadı!" << endl;
			break;
		}
		case 7: {
			int hesapNo;
			double miktar;
			bool hesapBulundu = false;

			cout << "Hesap numarasını giriniz: "; cin >> hesapNo;
			cout << "Ne kadar Dogecoin satmak istediğinizi giriniz: "; cin >> miktar;

			for (int i = 0; i < hesapSayisi; i++) {
				if (hesaplar[i].hesapNumarasiniSorgula() == hesapNo) {
					int gun, ay, yil;
					suankiTarihiAl(gun, ay, yil);

					bool isGenelHesap = hesaplar[i].hesapTuruSorgula();


					if (isGenelHesap) {
						hesaplar[i].genelHesapGuncelle(gun);
					}
					else {
						hesaplar[i].ozelHesapGuncelle(gun);
					}


					double mevcutDogecoinDegeri = hesaplar[i].dogecoinBakiyeSorgula();

					if (mevcutDogecoinDegeri >= miktar) {
						hesaplar[i].dogecoinDegeriBelirle(mevcutDogecoinDegeri - miktar);
						hesaplar[i].paraYatir(miktar);
						cout << miktar << " Dogecoin " << hesapNo << " numaralı hesaptan satıldı." << endl;
					}
					else {
						cout << "Yetersiz Dogecoin bakiyesi!" << endl;
					}

					hesapBulundu = true;
					break;
				}
			}

			if (!hesapBulundu) cout << "Hesap bulunamadı!" << endl;
			break;
		}
		case 8: {
			cout << "Tarih: " << setw(2) << setfill('0') << gun << "/" << setw(2) << setfill('0') << ay << "/" << yil << endl;
			break;
		}
		case 9: {
			int ilerlemekIcinGunSayisi;
			cout << "Ilerlemek istediginiz gun sayisini giriniz: "; cin >> ilerlemekIcinGunSayisi;
			tarihiIlerlet(gun, ay, yil, ilerlemekIcinGunSayisi);

			for (int i = 0; i < hesapSayisi; i++) {
				int gun, ay, yil;
				suankiTarihiAl(gun, ay, yil);

				if (hesaplar[i].hesapTuruSorgula()) {
					hesaplar[i].genelHesapGuncelle(gun);
				}
				else {
					hesaplar[i].ozelHesapGuncelle(gun);
				}
			}
			break;
		}
		case 10: {
			int hesapNo;
			bool hesapBulundu = false;

			cout << "Hesap numarasını giriniz: "; cin >> hesapNo;

			for (int i = 0; i < hesapSayisi; i++) {
				if (hesaplar[i].hesapNumarasiniSorgula() == hesapNo) {
					int gun, ay, yil;
					suankiTarihiAl(gun, ay, yil);

					double hesapKar = hesaplar[i].kar(gun, ay, yil);
					cout << "Hesap " << hesapNo << " için kar/zarar: " << hesapKar << endl;

					hesapBulundu = true;
					break;
				}
			}

			if (!hesapBulundu) cout << "Hesap bulunamadı!" << endl;
			break;
		}
		case 11: {
			cout << "Programdan çıkılıyor..." << endl;
			break;
		}
		}
	} while (secim != 11);
	system("pause");
	return 0;
}