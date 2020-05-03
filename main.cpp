#include <iostream>
#include <fstream>

using namespace std;

class Nod {

private:
    int info;
    Nod *next;

public:
    Nod(int info = 0, Nod *next = NULL) {
        this->info = info;
        this->next = next;
    }
    Nod(Nod &nod) {
        this->info = nod.info;
        this->next = nod.next;
    }
    virtual ~Nod() {}

    int getInfo() {
        return this->info;
    }
    void setInfo(int info) {
        this->info = info;
    }
    virtual Nod *getNext() {
        return this->next;
    }
    virtual void setNext(Nod *next) {
        this->next = next;
    }
};

class NodMarcaj : public Nod {
    int prio;
    NodMarcaj *next;

public:
    NodMarcaj(int info, int prio, NodMarcaj *next = NULL) : Nod(info) {
        this->next = next;
        this->prio = prio;
    }

    NodMarcaj *getNext() {
        return this->next;
    }
    void setNext(NodMarcaj *next) {
        this->next = next;
    }
    int getPrio() {
        return this->prio;
    }
    void setPrio(int prio) {
        this->prio = prio;
    }
};

class Coada {

protected:
    Nod *prim = NULL, *ultim = NULL;
    int dimMax; // dimensiunea maxima a cozii
    int length;

public:
    Coada(int dimMax = 0) {
        this->dimMax = dimMax;
        prim = NULL;
        ultim = NULL;
        length = 0;
    }
    Coada(const Coada &coada) {
        dimMax = coada.getDimMax();
        length = coada.getLength();

        Nod *p = coada.getPrim();
        while (p != NULL) {
            insereaza(p->getInfo());
            p = p->getNext();
        }
    }
    ~Coada() {
        Nod *temp, *p = prim;
        while(p != NULL) {
            temp = p;
            p = p->getNext();
            delete temp;
        }
        prim = NULL;
        ultim = NULL;
    }

    void insereaza(int value) {
        if (length == dimMax) {
            cout<<"Coada este plina.\n";
            return;
        }

        Nod *nod = new Nod(value);
        if (prim == NULL && ultim == NULL) {
            prim = nod;
            ultim = nod;
        } else {
            this->ultim->setNext(nod);
            ultim = nod;
        }
        length++;
    }

    int top() {
        return prim->getInfo();
    }

    void pop() {
        if (length == 0) {
            cout<<"Coada este deja goala.\n";
            return;
        }

        Nod *temp = prim;
        prim = prim->getNext();

        if (length == 1) {
            ultim = NULL;
        }

        delete temp;

        length--;
    }

    void goleste() {
        while (length > 0) {
            pop();
        }
    }

    int getDimMax() const {
        return dimMax;
    }
    void setDimMax(int dimMax) {
        this->dimMax = dimMax;
    }
    int getLength() const {
        return length;
    }
    virtual Nod* getPrim() const {
        return prim;
    }
    virtual Nod* getUltim() const {
        return ultim;
    }

    friend istream &operator >> (istream &in, Coada &aux);
    friend ostream &operator << (ostream &out, Coada &auxo);
    friend bool operator== (Coada &c1, Coada &c2);
};

bool operator== (Coada &c1, Coada &c2) {
    /// doua cozi sunt egale daca au aceleasi elemente in aceeasi ordine (ignoram valorile prioritatilor)
    if (c1.getLength() != c2.getLength()) {
        return false;
    }
    Nod *p1 = c1.getPrim();
    Nod *p2 = c2.getPrim();
    while (p1 != NULL && p2 != NULL) {
        if (p1->getInfo() != p2->getInfo()) {
            return false;
        }
        p1 = p1->getNext();
        p2 = p2->getNext();
    }

    return true;
}

istream &operator >> (istream &in, Coada &aux) {
    int dimMax, nr;
    in>>dimMax;

    aux = Coada(dimMax);

    while (in >> nr) {
        aux.insereaza(nr);
    }

    return in;
}

ostream &operator << (ostream &out, Coada &auxo) {
    Nod *r;
    r = auxo.prim;

    if (r == NULL) {
        out<<"Coada este goala.\n";
        return out;
    }

    while (r != NULL) {
        out<<r->getInfo()<<" ";
        r = r->getNext();
    }
    out<<endl;
    return out;
}

class Deque : public Coada {
private:
protected:
    using Coada::pop; /// in loc de pop, se vor folosi metodele stergeInceput sau stergeSfarsit
    using Coada::insereaza;
public:

    Deque() {}
    Deque(int dimMax) : Coada(dimMax) {}

    virtual void stergeInceput() {
        pop();
    }

    virtual void stergeSfarsit() {
        if (length == 0) {
            cout<<"Coada este deja goala.\n";
            return;
        }

        Nod *p = prim;
        while(p->getNext() != ultim) {
            p = p->getNext();
        }

        Nod *temp = ultim;
        ultim = p;
        ultim->setNext(NULL);
        delete temp;

        length--;
    }

    void insereazaInceput(int val) {
        if (length == 0) {
            insereaza(val);
            return;
        }

        Nod *nod = new Nod(val);
        nod->setNext(prim);
        prim = nod;

        length++;
    }

    void insereazaSfarsit(int val) {
        insereaza(val);
    }
};

class DequeMarcaj : public Deque {
private:
    using Deque::insereazaInceput; /// in loc de insereazaInceput si insereazaSfarsit se va folosi insereaza(val, prio)
    using Deque::insereazaSfarsit;
    NodMarcaj *prim, *ultim;

public:
    DequeMarcaj(int dimMax = 0) : Deque(dimMax) {
        prim = NULL;
        ultim = NULL;
    }

    void stergeInceput() {
        if (length == 0) {
            cout<<"Coada este deja goala.\n";
            return;
        }

        NodMarcaj *temp = prim;
        prim = prim->getNext();

        if (length == 1) {
            ultim = NULL;
        }

        delete temp;

        length--;
    }

    void stergeSfarsit() {
        if (length == 0) {
            cout<<"Coada este deja goala.\n";
            return;
        }

        NodMarcaj *p = prim;
        while(p->getNext() != ultim) {
            p = p->getNext();
        }

        NodMarcaj *temp = ultim;
        ultim = p;
        ultim->setNext(NULL);
        delete temp;

        length--;
    }

    void insereaza(int val, int prio) {
        if (length == dimMax) {
            cout<<"Coada este plina.\n";
            return;
        }

        NodMarcaj *nod = new NodMarcaj(val, prio);
        if (prim == NULL && ultim == NULL) {
            prim = nod;
            ultim = nod;
        } else {
            // inserare in functie de prioritate
            NodMarcaj *p = prim, *prec;
            while(p != NULL && p->getPrio() > prio) {
                prec = p;
                p = p->getNext();
            }

            if (p == prim) {
                // inserare pe prima pozitie
                nod->setNext(prim);
                prim = nod;
            }
            if (p == NULL) {
                // inserare pe ultima pozitie
                this->ultim->setNext(nod);
                ultim = nod;
            }

            nod->setNext(p);
            prec->setNext(nod);
        }

        length++;
    }

    void afis() {
        NodMarcaj *p = prim;
        while (p != NULL) {
            cout<<"("<<p->getInfo()<<", "<<p->getPrio()<<")  ";
            p = p->getNext();
        }
        cout<<endl;
    }

    NodMarcaj* getPrim() const {
        return prim;
    }
    NodMarcaj* getUltim() const {
        return ultim;
    }
    void setPrim(NodMarcaj* prim) {
        this->prim = prim;
    }
    void setUltim(NodMarcaj* ultim) {
        this->ultim = ultim;
    }

    friend istream &operator >> (istream &in, DequeMarcaj &aux);
    friend ostream &operator << (ostream &out, DequeMarcaj &auxo);
};

istream &operator >> (istream &in, DequeMarcaj &aux) {
    int dimMax, nr, prio;
    in>>dimMax;

    aux.setDimMax(dimMax);

    while (in >> nr >> prio) {
        aux.insereaza(nr, prio);
    }

    return in;
}

ostream &operator << (ostream &out, DequeMarcaj &auxo) {
    NodMarcaj *r;
    r = auxo.prim;

    if (r == NULL) {
        out<<"Coada este goala.\n";
        return out;
    }

    while (r != NULL) {
        out<<"("<<r->getInfo()<<", "<<r->getPrio()<<")  ";
        r = r->getNext();
    }
    out<<endl;
    return out;
}

int main()
{
    ifstream f("coada.in");
    ifstream f2("marcaj.in");

    Coada coada = Coada(10);
    coada.insereaza(1);
    coada.insereaza(2);
    coada.insereaza(3);
    cout<<coada.top()<<endl;
    coada.pop();
    cout<<coada;
    coada.goleste();
    cout<<coada;
    coada.pop();

    Coada coada2 = Coada();
    cout<<"coada si coada2 sunt egale: "<<(coada == coada2)<<endl;

    Deque deq = Deque(9);
    deq.insereazaSfarsit(13);
    deq.insereazaInceput(2);
    deq.insereazaInceput(3);
    deq.insereazaInceput(4);
    deq.insereazaSfarsit(5);
    cout<<deq;
    deq.stergeInceput();
    deq.stergeSfarsit();
    cout<<deq;

    Deque deq2 = Deque(8);
    deq2.insereazaSfarsit(3);
    deq2.insereazaSfarsit(2);
    deq2.insereazaSfarsit(13);
    cout<<deq2;
    cout<<"deq si deq2 sunt egale: "<<(deq == deq2)<<endl;
    deq2.stergeInceput();
    cout<<deq2;
    cout<<"deq si deq2 sunt egale: "<<(deq == deq2)<<endl;

    DequeMarcaj marcaj;
    f2>>marcaj;
    marcaj.insereaza(12, 5);
    marcaj.insereaza(20, 2);
    marcaj.insereaza(3, 3);
    cout<<marcaj;
    marcaj.stergeSfarsit();
    marcaj.stergeInceput();
    cout<<marcaj;

    DequeMarcaj marcaj2 = DequeMarcaj(100);
    marcaj2.insereaza(12, 5);
    cout<<"marcaj si marcaj2 sunt egale: "<<(marcaj == marcaj2)<<endl;
    cout<<"coada si marcaj2 sunt egale: "<<(coada == marcaj2)<<endl;

    return 0;
}
