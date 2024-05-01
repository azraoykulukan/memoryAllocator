# BIL 222 Project2: Memory Allocator


## Bilinmesi gerekenler



* Dynamic memory allocation
* sbrk()
* Linked lists
* struct, typedef
* Memory alignment
* Heap


## Özet

Bu ödevde bir tane C programları için memory allocator yapmanız istenmektedir.


## Detay


### Kodlama

Mymalloc.c içerisinde verilen fonksiyonların hepsini implement etmeniz ve özellikle aşağıdaki mymmaloc() ve myfree() fonksiyonalitesini sağlamanız beklenmektedir.

**void *mymalloc(size_t size) **şu şekilde çalışmalıdır.



* Eğer ilk defa çağrılıyorsa yada yeterli alan yoksa, 
    * sbrk kullanılarak HEAP_SIZE byte miktarinda heap alanı genişletilmeli 
    * ve bir tane free block oluşturulmalı
    * Ve free liste eklenmeli
* size  16 nın katı olacak şekilde yukarı yuvarlama yapılmalı
    * Size bilgisi 16nın katı cinsinden
* İstenilen alanı karşılayacak bir tane free block bulunup (seçilen stratejiye göre), bu alan iki bloğa split edilerek **(split fonksiyonunu implement etmeniz gerekiyor)**, birisi free liste eklenmeli diğerinin (**size** kadar alanın)  başlangıç adresi (data[]) return edilmelidir.

**void *myfree(void *p) **



* p ile verilen alan free edilerek free liste eklenmelidir.
* Eğer coalescing(birleştirme) yapılabiliyorsa (bir önceki ve bir sonraki blokla) yapılmalıdır. 
* Free list `listtype` a göre iki tür olabilmektedir: Address bilgisine göre sıralı veya lifo

**void printheap()**

Heap deki blokların metadatalarını tek tek yazdırmalı (dolu yada boş fark etmiyor). 

Yazdırırken çıktı şu formatta olmalı:

Blocks

Free: 1

Size: xxx  

---------------

Free: 0

Size: xxx  

---------------


### Rapor

Programınızı farklı inputlarla çalıştırarak external fragmentationın hangi stratejide daha iyi hangisinde daha kötü olduğunun çok kısa açıklamasını Rapor.txt içerisinde ekleyiniz. 
