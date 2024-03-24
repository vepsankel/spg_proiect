SPG - Proiect - Zaharov EVghenii 324C1
Acest proiect are ca scop redarea suprafetei apei folosind transformarile Fourier.
1. Generarea transformarilor.
Transformarile stau la baza redarii realistice a suprafetei. Untrucat apa realistica poate fi redata folosind doar mai multe functii sinusoide, si calcularea lor in shadere poate consuma prea multa memorie, transformata Fourier ne ajuta sa reducem numarul de calculari. 
In proiectul meu eu am definit clasa Fourier. In ea utilizatorul poate defini un sample - colectie de numere complexe, care descriu deplasarea vertexului in plan perpendicular frontului de unda. 
Exemplu:
![SPG-img1](https://github.com/vepsankel/spg_proiect/assets/69473728/69f0b7a8-96bc-445c-b67f-855b8d2289c7)

 
Aici fiecare vector reprezinta un sample - un numar complex, corespunzator deplasarii unui vertex. Un vertex va trece prin toate aceste pozitii. La fel, numere complexe asigura posibilitatea redarii mai precise (deoarece vertexii cor fi concentrati in locuri in care un val se schimba mai brusc) si redarea unor forme ce nu reprezinta o functie, spre exemplu:
 
2. Salvarea transformarilor.
Transformari simple aproape nu ne usureaza redarea - sigur sunt o varianta mai buna decat sinusoide, dar nici nu ideala. Pentru a le folosi si mai eficient, putem salva rezultate a simularii suprafetei, scriind informatie despre inaltime intr-o textura. Pentru asta exista clasa Wave si shadere DrawToText. In ele sunt definite metode pentru a desena harta deplasarii intr-o textura dintr-o transformata. 
Texture generate arata in felul urmator (prima este textura unei unde-zgomot, a doua - undei principale):

![SPG-img2](https://github.com/vepsankel/spg_proiect/assets/69473728/81576f31-9e71-4581-96a7-bfb371a9c30d)

![SPG-img22](https://github.com/vepsankel/spg_proiect/assets/69473728/1b0d492d-3b86-4e7b-abdd-aa2d6a394836)

3. Suprafata pentru redarea transformarii.
Pentru a reda o suprafata apei avem nevoie de suprafata. Suprafata este definita in clasa Surface si este de fapt un plan, format de mai multi vertexi (numarul lor se poate regla). Cu cat mai mare este numar - cu atat mai multi vertexi vor exista si atat mai precisa va fi suprafata obtinuta, in acelasi timp va creste si memoria necesara redarii. Pentru a reda suprafata in shadere (Surface) sunt transmise texturi obtinute din pasii anteriori si instructii de a le citi.

4. Folosirea texturilor pentru a reda suprafata.
Impreuna cu texturele propriu zise in shadere sunt trimise si matricile necesare citirii. 
In general sunt 3 pentru fiecare textura:
	Prima matrice este aplicata coordonatelor texturii din suprafata si in urma inmultirii se obtine pozitia de citire din textura. De exemplu, daca un vertex are coordonata de textura 0.5, 0.5 (adica se afla in mijlocul planului) si prima matrice asociata acestei texturi are valoare [ [2,0] [0,2] ], atunci pozitia initiala de citire din textura va fi 1,1 (adica sfarsitul texturii). In acest exemplu matricea respectiva a cauzat "micsorarea" unui val - daca in mod normal un val ocupa intreg plan, acum un val acopera doar jumatate. La fel, matricea [ [0, 1] [1, 0] ] va cauza, spre exemplu, rotirea valului.
	A doua matrice este ca prima, dar este inmultita cu timp inainte de a fi aplicata coordonatelor texturii. Prin aceasta matrice putem controla, cum sunt propagate valurile (viteza si plan).
Suma acestor produse va determina coordonate de citire. Dupa ce va fi citita deplasarea din textura respectiva, ea va fi inmultita cu a 3a matrice, prin care putem controla inaltime/unghi al valului.
