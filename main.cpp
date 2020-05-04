#include <iostream>
#include <fstream>

using namespace std;


class Nod {

private:
    int info;

protected:
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

public:
    NodMarcaj(int info, int prio, NodMarcaj *next = NULL) : Nod(info) {
        this->next = next;
        this->prio = prio;
    }

    NodMarcaj *getNext() {
        return static_cast<NodMarcaj*>(next);
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
        length = 0;

        Nod *p = coada.getPrim();
        while (p != NULL) {
            insereazaSfarsit(p->getInfo());
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

    void insereazaSfarsit(int value) {
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

    void stergeInceput() {
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
            stergeInceput();
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
    void setPrim(Nod *prim) {
        this->prim = prim;
    }
    void setUltim(Nod *ultim) {
        this->ultim = ultim;
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
        aux.insereazaSfarsit(nr);
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

public:

    Deque() {}
    Deque(int dimMax) : Coada(dimMax) {}

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
            insereazaSfarsit(val);
            return;
        }

        Nod *nod = new Nod(val);
        nod->setNext(prim);
        prim = nod;

        length++;
    }
};

class DequeMarcaj : public Deque {
private:
    using Deque::insereazaInceput; /// in loc de insereazaInceput si insereazaSfarsit se va folosi insereaza(val, prio)
    using Deque::insereazaSfarsit;

public:
    DequeMarcaj(int dimMax = 0) : Deque(dimMax) {
        prim = NULL;
        ultim = NULL;
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
            NodMarcaj *p = getPrim(), *prec;
            while(p != NULL && p->getPrio() > prio) {
                prec = p;
                p = p->getNext();
            }

            if (p == getPrim()) {
                // inserare pe prima pozitie
                nod->setNext(prim);
                prim = nod;
            } else if (p == NULL) {
                // inserare pe ultima pozitie
                this->ultim->setNext(nod);
                ultim = nod;
            } else {
                nod->setNext(p);
                prec->setNext(nod);
            }
        }

        length++;
    }

    void afis() {
        NodMarcaj *p = static_cast<NodMarcaj*>(prim);
        while (p != NULL) {
            cout<<"("<<p->getInfo()<<", "<<p->getPrio()<<")  ";
            p = p->getNext();
        }
        cout<<endl;
    }

    NodMarcaj* getPrim() const {
        return static_cast<NodMarcaj*>(prim);
    }
    NodMarcaj* getUltim() const {
        return static_cast<NodMarcaj*>(ultim);
    }
    void setPrim(Nod* prim) {
        this->prim = prim;
    }
    void setUltim(Nod* ultim) {
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
    r = auxo.getPrim();

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

void testCoada(Coada q) {
    cout<<"coada: "<<q;
    cout<<"top: "<<q.top()<<endl;
    q.stergeInceput();
    cout<<"coada dupa stergeInceput(): "<<q;
    q.goleste();
    cout<<"coada dupa goleste(): "<<q;
    cout<<endl;
}

void testDeque(Deque q) {
    cout<<"deque: "<<q;
    q.stergeInceput();
    cout<<"deque dupa stergeInceput(): "<<q;
    q.stergeSfarsit();
    cout<<"coada dupa stergeSfarsit(): "<<q;
    cout<<endl;
}

int main()
{
    ifstream f("coada.in");
    ifstream f2("marcaj.in");

    Coada coada = Coada(10);
    coada.insereazaSfarsit(1);
    coada.insereazaSfarsit(2);
    coada.insereazaSfarsit(3);

    testCoada(coada);

    Deque deq = Deque(9);
    deq.insereazaSfarsit(13);
    deq.insereazaInceput(2);
    deq.insereazaInceput(3);
    deq.insereazaInceput(4);
    deq.insereazaSfarsit(5);

    testCoada(deq);
    testDeque(deq);

    DequeMarcaj marcaj;
    f2>>marcaj;
    marcaj.insereaza(12, 5);
    marcaj.insereaza(20, 2);
    marcaj.insereaza(3, 3);

    testCoada(marcaj);
    testDeque(marcaj);

    Coada coada2 = Coada(12);
    coada2.insereazaSfarsit(1);
    coada2.insereazaSfarsit(2);
    coada2.insereazaSfarsit(3);
    cout<<"coada2 = "<<coada2;

    DequeMarcaj marcaj2 = DequeMarcaj(13);
    marcaj2.insereaza(2,10);
    marcaj2.insereaza(3,5);
    marcaj2.insereaza(1,20);
    cout<<"marcaj2 = "<<marcaj2;

    cout<<"coada2 si marcaj2 sunt egale: "<<(coada2 == marcaj2)<<endl;

    return 0;
}
