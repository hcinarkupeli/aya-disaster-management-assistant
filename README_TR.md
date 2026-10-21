# AYA - Afet Yönetim Asistanı

AYA; yerel sarsıntı algılama, erişilebilir uyarı, akıllı bileklik, kapıdan giriş-çıkış sayımı ve mobil arayüzü bir araya getiren öğrenci mühendislik projesidir. Proje, 2025 TÜBİTAK 2204-B ulusal finalinde Yazılım alanında Teşvik Ödülü almıştır.

**Ekip:** Hasan Çınar Küpeli ve Luna Hazal Zafran. Hasan'ın belgelenen katkıları gömülü yazılım, programlama ve sensör entegrasyonudur. Proje ekip çalışmasıdır.

Depoda özgün prototip fotoğrafları, altı tarihsel Arduino kodu, tavan kutusunun STL dosyası, uygulama ekranları, teknik açıklamalar ve çalıştırılabilir veri analizi bulunur. İngilizce ana açıklama: [README.md](README.md).

## Çalıştırma - Windows

```powershell
python -m venv .venv
.\.venv\Scripts\python.exe -m pip install -r requirements.txt
.\.venv\Scripts\python.exe analysis/risk_analysis.py --output results/local/risk_analysis.json
.\.venv\Scripts\python.exe -m unittest discover -s tests -v
```

Tarihsel veri dosyasında 2.000 satır, 294 farklı nabız-sıcaklık çifti ve farklı risk etiketleri atanmış 51 çift bulunmaktadır. Aynı çiftler eğitim ve test kümelerine dağılmaz. Sınıflandırma, dosyadaki sayısal etiketlerle sınırlıdır; klinik triyaj veya sağlık kararı değildir.

2025 prototipi ile 2026 depo bakımında oluşturulan analiz ve C++ mantık modülleri ayrı belgelenmiştir. Yazılım kontrolleri fiziksel cihaz testi anlamına gelmez. Tarihsel kodların sınırlılıkları [arşiv açıklamasında](archive/README.md) verilmiştir.

MIT App Inventor `.aia` kaynağı bulunmadığı için uygulama, özgün ekran görüntüleriyle belgelenmiştir. Bulunan kaynaklar dışındaki APK, sensör kaydı, klinik doğruluk, deprem tahmini veya resmî operasyon entegrasyonu iddia edilmemektedir.

## GitHub'a yükleme

Yeni depo adı: `aya-disaster-management-assistant`. ZIP'i bilgisayarda açın. Bu dosya ve `README.md` ile aynı düzeydeki tüm dosya ve klasörleri, klasör yapısını koruyarak deponun ana dizinine yükleyin. ZIP'in kendisini yüklemek gerekmez. İlk kayıt açıklaması: `Add AYA research portfolio, prototype sources and documentation`.
