# TCP-Soket-Programlama
Server ve Client ile banka uygulaması
TCP Server ve TCP Client olarak iki konsol uygulaması geliştirilecek olup,
Server'da önceden oluşturulmuş farklı bankalara ait kullanıcı hesapları, kartları ve
hesaplara ait bakiyeler (herhangi bir .txt dosyasına kaydedilmiş olabilir, yada database
kullanılabilir.) üzerinde işlemler server tarafında yapılacaktır.
TCP server aracılığı Client'tan gelen isteğe göre para transferi gerçekleştirilecektir.
Kullanıcılar Client uygulamasında kullanıcı adı ve parola ile giriş yapıp bakiyesini
görebilecek ve para göndermek için sistemde kayıtlı olan diğer müşteri numarasını,
isim-soyadını ve göndereceği miktarı yazarak yeterli bakiye varsa belirlenen kullanıcıya
transfer yapılacaktır. Eğer yeterli bakiye yok ise işlem gerçekleştirilemeyecektir. Ayrıca
kendi bakiyesinden para da çekebilecektir ve bakiyesine para yatırabilecektir. Bir
hesaptan diğer hesaba transfer yapılırken eğer transfer yapılan hesap farklı bankaya
ait ise önceden belirlenmiş miktar kadar kesinti yapılacaktır.
