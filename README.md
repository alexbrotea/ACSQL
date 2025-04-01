    La taskul 1, prima functie, trim, a fost creata de mine pentru a elimina spatiile de la string-uri,
ce ma impiedicau initial sa trec prima parte a taskului 1, subpunctul 1.1. Cand ma refer la spatii, ma
refer inclusiv atat la cele de dinainte, cat si la cele de dupa care pot aparea la un string. Ulterior,
voi apela aceasta functie mai departe in implementarea mea.
    Urmatoarea functie, citeste_secretariat, e folosita de mine ca sa setez toate campurile(listele de
studenti, materii si inrolari) initial cu NULL. Dupa care, citesc fisierul linie cu linie, analizand
fiecare sectiune in parte, [STUDENTI], [MATERII] si [INROLARI]. In functie de sectiunea curenta, datele
sunt interpretate si stocate in structuri corespunzatoare. Aloc dinamic memorie pentru fiecare entitate 
citita si o adaug in lista corespunzatoare din structura secretariat, verific formatul si datele pentru
a preveni erorile, cum ar fi suprascrierea memoriei sau alocari nereusite. Dupa care, calculez media
generala pentru fiecare student, utilizand datele din sectiunea [INROLARI]. Aceasta implica parcurgerea
inrolarilor si trecerea notelor pentru fiecare student in parte. La final, ma asigur ca resursele sunt
gestionate corespunzator, inchizand fisierul la final si returnand structura ceruta. De asemenea, tot
in aceasta functie, am apelat pentru a ma asigura ca imi iese taskul bine la functia trim.
    Functia adauga_student permite adaugarea dinamica a unui student in structura existenta, actualizand
numarul total de studenti si asigurand alocarea corecta a memoriei pentru noul student. Este o componenta
esentiala pentru extinderea ulterioara a listei de studenti.
    Functia elibereaza_secretariat se ocupa de eliberarea memoriei alocate pentru structurile dinamic
create, pentru a preveni scurgerile de memorie, mai ales atunci cand structurile contin alocari complexe
(de exemplu, stringuri dinamice pentru materii). Am implementat un parser pentru sectiunile [STUDENTI],
[MATERII] si [INROLARI] din fisierul de intrare, asigurandu-ma ca datele sunt corect interpretate si stocate.
    In cadrul taskului 3, am inceput prin pregatirea datelor, extinzand vectorul de initializare (IV)
pentru a se potrivi cu dimensiunea blocurilor ce urmau sa fie criptate. Ulterior, am aplicat transformari
iterativ, bloc cu bloc. Fiecare bloc a fost procesat prin operatii XOR cu cheia furnizata, urmat de
transformari P-Box si S-Box pentru a introduce substitutie si difuzie in criptare. Acest proces iterativ
a asigurat ca fiecare parte a textului criptat depinde nu doar de textul original, ci si de blocurile
anterioare.
    Odata ce toate blocurile au fost criptate, rezultatul final a fost salvat intr-un fisier binar,
permitand stocarea datelor intr-un format compact. 