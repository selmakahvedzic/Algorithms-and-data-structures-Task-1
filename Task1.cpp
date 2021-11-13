#include <iostream>
#include <stdexcept>
#include <string>
#include <typeinfo>
using namespace std;



//Određene stvari koje se ponavljaju sam kopirala iz pripremne 2
template <typename Tip>
class Lista
{
public:
    Lista() {}
    virtual ~Lista() {};
    virtual int brojElemenata() const=0;
    virtual Tip trenutni() const=0;
    virtual Tip &trenutni()=0;
    virtual bool prethodni()=0;
    virtual bool sljedeci()=0;
    virtual void pocetak()=0;
    virtual void kraj()=0;
    virtual void obrisi()=0;
    virtual void dodajIspred(const Tip &el)=0;
    virtual void dodajIza(const Tip &el)=0;
    virtual Tip &operator[](int i)=0;
    virtual Tip operator[](int i) const=0;
};

template <typename Tip>
class Iterator;


template<typename Tip>
class DvostrukaLista: public Lista<Tip>
{
    int brElemenata;
    struct Cvor {
        Tip element;
        Cvor *sljedeci, *prethodni;
    };

    Cvor *t, *poslj, *poc;

public:
    friend class Iterator<Tip>;
    DvostrukaLista() : brElemenata(0), t(nullptr), poslj(nullptr), poc(nullptr) {}
    ~DvostrukaLista()
    {
        while(poc!=nullptr) {
            t=poc;
            poc=poc->sljedeci;
            delete t;
        }
    }


    DvostrukaLista(const DvostrukaLista<Tip> &l);

    int brojElemenata() const override
    {
        return brElemenata;
    }

    Tip &trenutni() override
    {
        return t->element;
    }

    Tip trenutni() const override
    {
        return t->element;
    }

    bool prethodni()
    {
        if(t==poc) return false;
        t=t->prethodni;
        return true;
    }

    bool sljedeci() override
    {
        if(t==poslj) return false;
        t=t->sljedeci;
        return true;
    }

    void pocetak() override
    {
        if(brElemenata==0) throw "Izuzetak!";
        t=poc;
    }


    void kraj() override
    {
        if(brElemenata==0) throw "Izuzetak!";
        t=poslj;

    }
    void obrisi() override;
    void dodajIspred(const Tip& el) override ;
    void dodajIza(const Tip& el) override ;
    Tip operator [] (int i) const override
    {
        Cvor *pom=poc;
        for(int j=0; j<i; i++) pom=pom->sljedeci;
        return pom->element;
    }
    Tip &operator [] (int i) override
    {
        Cvor *pom=poc;
        for(int j=0; j<i; j++) pom=pom->sljedeci;
        return pom->element;
    }


    DvostrukaLista& operator=(const DvostrukaLista<Tip> &l);
};



template<typename Tip>
void DvostrukaLista<Tip>::obrisi()
{
    if(poc==nullptr) throw "Izuzetak!";
    if(brElemenata==1) {
        delete t;
        poc=nullptr;
        poslj=nullptr;
        t=nullptr;
    } else if(t==poc) {
        poc=t->sljedeci;
        poc->prethodni=nullptr;
        delete t;
        t=poc;
    } else if(t==poslj) {
        poslj=t->prethodni;
        poslj->sljedeci=nullptr;
        delete t;
        t=poslj;
    } else {
        Cvor *pom=poc;
        while(pom->sljedeci!=t) pom=pom->sljedeci;
        t->prethodni->sljedeci=t->sljedeci;
        t->sljedeci->prethodni=t->prethodni;
        delete t;
        t=pom->sljedeci;
    }
    brElemenata--;
}


template<typename Tip>
void DvostrukaLista<Tip>::dodajIspred(const Tip &element)
{
    Cvor *novi= new Cvor();
    novi->element=element;
    novi->sljedeci=nullptr;
    if(brElemenata==0) {
        poc=novi;
        poslj=novi;
        t=novi;
    } else if(t==poc) {
        poc->prethodni=novi;
        novi->sljedeci=poc;
        poc=novi;
    } else {
        Cvor *pom=t->prethodni;
        pom->sljedeci=novi;
        novi->prethodni=pom;
        novi->sljedeci=t;
        t->prethodni=novi;
    }
    brElemenata++;
}


template<typename Tip>
void DvostrukaLista<Tip>::dodajIza(const Tip &element)
{
    Cvor *novi= new Cvor();
    novi->element=element;
    novi->sljedeci=nullptr;
    if(brElemenata==0) {
        poc=novi;
        poslj=novi;
        t=novi;
    } else if(t==poslj) {
        poslj->sljedeci=novi;
        novi->prethodni=poslj;
        poslj=novi;
    } else {
        Cvor *pom=t->sljedeci;
        novi->prethodni=t;
        t->sljedeci=novi;
        pom->prethodni=novi;
        novi->sljedeci=pom;
    }
    brElemenata++;
}



template<typename Tip>
DvostrukaLista<Tip>::DvostrukaLista(const DvostrukaLista<Tip> &l)
{
    Cvor *pom1=l.poc;
    Cvor *pom2=nullptr;
    brElemenata=l.brElemenata;
    while(pom1!=nullptr) {
        Cvor *novi=new Cvor;
        novi->element=pom1->element;
        if(pom1==l.poc) poc = novi;
        else {
            pom2->sljedeci=novi;
            novi->prethodni=pom2;
        }
        pom2=novi;
        if(pom1==l.t) t=novi;
        pom1=pom1->sljedeci;
    }
    poslj=pom2;
    pom2->sljedeci=nullptr;
}



template <typename Tip>
DvostrukaLista<Tip> &DvostrukaLista<Tip>::operator=(const DvostrukaLista<Tip> &l)
{
    if(this==&l) return *this;
    while(poc!=nullptr) {
        t=poc;
        poc=poc->sljedeci;
        delete t;
    }
    Cvor *pom1=l.poc;
    Cvor *pom2=nullptr;
    brElemenata=l.brElemenata;
    while(pom1!=nullptr) {
        Cvor *novi=new Cvor;
        novi->element=pom1->element;
        if(pom1==l.poc) poc=novi;
        else {
            pom2->sljedeci=novi;
            novi->prethodni=pom2;
        }
        pom2=novi;
        if(pom1==l.t) t=novi;
        pom1=pom1->sljedeci;
    }
    poslj=pom2;
    pom2->sljedeci=nullptr;

    return *this;
}

//Iterator
template <typename Tip>
class Iterator
{
    const DvostrukaLista<Tip>*dvLista;
    typename DvostrukaLista<Tip>::Cvor *t;

public:

    Iterator(const DvostrukaLista<Tip>&dl):t(dl.t) {}
    Iterator(const Lista<Tip>&l) :dvLista((DvostrukaLista<Tip>*)&l),t((*dvLista).t) {}

    Tip trenutni() {
        if(dvLista->brElemenata==0) throw("Prazna Lista!");
        return t->element;
    }
    
    void pocetak() {
        if(dvLista->brElemenata==0) throw("Prazna Lista!");
        t=dvLista->poc;
    }

    void kraj() {
        if(dvLista->brElemenata==0) throw("Prazna Lista!");
        t=dvLista->poslj;
    }
    
    bool prethodni() {
        if(dvLista->brElemenata==0) throw("Prazna Lista!");
        if(t->prethodni==nullptr) return false;
        t=t->prethodni;
        return true;
    }
    
    bool sljedeci() {
        if(dvLista->brElemenata==0) throw("Prazna Lista!"); 
        if(t->sljedeci==nullptr) return false;
        t=t->sljedeci;
        return true;
    }
    
};

//metoda dajMaksimum
template <typename Tip>
Tip dajMaksimum(const Lista<Tip> &n)
{
    Iterator <Tip> it(n);
    it.pocetak();
    Tip maksimum=it.trenutni();
    it.sljedeci();
    for(int k=0; k<n.brojElemenata()-1; k++) {
        if(it.trenutni()>maksimum)
            maksimum=it.trenutni();
        it.sljedeci();
    }
    return maksimum;
}


//TESTNE FUNKCIJE
//Kopirajuci konstruktor
void TestKopirajucegKonstruktora()
{
    DvostrukaLista<int> lista1;
    auto lista2(lista1);
    DvostrukaLista<char> lista3;
    DvostrukaLista<char> lista4(lista3);
    std::cout << "Test kopirajuceg konstruktora uspjesan! ";
}

//Obrisi
void TestMetodeObrisi()
{
    DvostrukaLista<int>lista1;
    std::cout<<lista1.brojElemenata()<<" ";
    lista1.dodajIspred('a');
    lista1.dodajIspred('b');
    std::cout<<lista1.brojElemenata()<<" ";
    lista1.obrisi();
    std::cout<<lista1.brojElemenata()<<std::endl;
}

//dajmax
void TestDajMaksimum()
{
    Lista<int>*lista;
    lista=new DvostrukaLista<int>;
    for(int i=0; i<100; i++) {
        lista->dodajIspred(i);
    }
}

//indeks
void TestOperatoraIndeks()
{
    DvostrukaLista<int>lista;
    lista.dodajIza(1);
    lista.dodajIza(2);
    std::cout<<lista[0]<<std::endl;
}

//dodajiza
void TestDodajIza()
{
    DvostrukaLista<int>lista;
    lista.dodajIza(10);
    lista.dodajIza(5);
    std::cout << lista.brojElemenata()<<std::endl;
}

//dodajispred
void TestDodajIspred()
{
    DvostrukaLista<int>lista;
    lista.dodajIspred(8);
    lista.dodajIspred(3);
    DvostrukaLista<int>lista2;
    lista2.dodajIspred(14);
    std::cout<<lista.brojElemenata()<<" "<<lista2.brojElemenata()<<std::endl;

}

//brelemenata
void TestMetodeBrojElemenata()
{
    DvostrukaLista<int>lista;
    std::cout<<lista.brojElemenata()<<std::endl;
    lista.dodajIspred(1);
    std::cout<<lista.brojElemenata()<<std::endl;

}

//prethodni
void TestMetodePrethodni()
{
    DvostrukaLista<int>lista;
    lista.dodajIza(6);
    lista.dodajIza(100);
    if(lista.prethodni()) {
        std::cout<<"Test uspješan!"<<std::endl;
    }

}

//Sljedeci
void TestMetodeSljedeci()
{
    DvostrukaLista<int>lista;
    lista.dodajIza(6);
    lista.dodajIza(100);
    if(lista.sljedeci()) {
        std::cout << "Test uspješan!"<<std::endl;
    }
}

//trenutni
void TestMetodeTrenutni()
{
    DvostrukaLista<int>lista;
    lista.dodajIspred(1);
    lista.dodajIza(2);
    std::cout<<lista.trenutni();

}

//pocetak
void TestMetodePocetak()
{
    DvostrukaLista<int>lista;
    lista.dodajIza(10);
    lista.dodajIza(80);
    lista.pocetak();
    std::cout << lista.trenutni()<<std::endl;
}

//Metoda kraj
void TestMetodeKraj()
{
    DvostrukaLista<int>lista;
    lista.dodajIza(10);
    lista.dodajIza(80);
    lista.kraj();
    std::cout<<lista.trenutni()<<std::endl;
}


int main()
{

    //Testirajmo i u mainu neke stvari
    Lista<int>*lista;
    lista=new DvostrukaLista<int>;
    for(int i=0; i<10000; i++) {
        lista->dodajIspred(i);
    }

    std::cout << dajMaksimum(*lista);
    delete lista;


    std::cout << std::endl;
    DvostrukaLista<int> dlista,dlista1;
    try {
        std::cout<<"Zadaća 1 Zadatak 1"<<std::endl;
    } catch(...) {
        cout<<"Izuzetak je uhvaćen."<<endl;
    }
    return 0;
}
