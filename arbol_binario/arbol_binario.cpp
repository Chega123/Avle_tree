#include <iostream>
>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

using namespace std;

#define ENDL cout<<endl;
#define COUNT 10
////////////////////////////////////////////////////////////////////////////////////////////////////////

struct CBinNode
{
    CBinNode(int _v)
    {
        value = _v;
        nodes[0] = nodes[1] = 0;
    }
    int value;
    int profundidad=0;
    int espacio_izq=0;
    int espacio_der = 0;
    int espacio_nulo = 0;
    CBinNode* nodes[2];
};

struct nodofila {
    int estado=0;
    CBinNode* nodo;
    nodofila* next=nullptr;
    int AVL_altura = 0;
    nodofila(CBinNode* x) {
        nodo = x;
    
    }
};
////////////////////////////////////////////////////////////////////////////////////////////////////////

class Stack {
public:
    void push(CBinNode* x);
    void pop();
    void print();
    int size();
    nodofila* temp = nullptr;
    nodofila* top = nullptr;

};

int Stack::size() {
    int count = 0;
    for (nodofila* i = this->top; i; i = i->next) {
        count += 1;
    }
    return count;
}

void Stack::push(CBinNode*x) {

    temp= new nodofila(x);
    temp->next = top;
    top = temp;

}

void Stack::pop() {
    if (top) {
        temp = top;
        top = top->next;
        temp = nullptr;
    }
    else { top = nullptr; }
}


///////////////////////////////////////////////////////////////////////

class Queue {
public:
    void push(CBinNode* x);
    void pop();
    void print();
    nodofila* temp = nullptr;
    nodofila* head = nullptr;
    nodofila* tail = nullptr;
};

void Queue::push(CBinNode* x) {
    if (head && tail) {
        tail->next = new nodofila(x);

        tail = tail->next;
    }
    else {
        head = new nodofila (x);
        tail = head;
    }
}

void Queue::pop() {
    if (head && tail) {
        temp = head;
        head = head->next;
        temp = nullptr;
    }
    else { head = nullptr; tail = nullptr; }

}


//////////////////////////////////////////////////////////////////////////


class CBinTree
{
public:
    CBinTree();
    ~CBinTree();
    bool Insert(int x);
    bool Remove(int x);
    void Print();
    void printTree(int x, int y, CBinNode* node, int index, sf::RenderWindow& window, sf::Font font);
    CBinNode* root;
private:
    bool Find(int x, CBinNode**& p, Stack& arr);
    bool Find_del(int x, CBinNode**& p, Stack& arr);
    CBinNode** Rep(CBinNode** p);
    void InOrder(CBinNode* x);
    void PostOrder(CBinNode* x);
    void PreOrder(CBinNode* x);
    void Inverse(CBinNode* x);
    void InOrder_manual(CBinNode* x);
    int Profundidad(CBinNode* x);
    void print2DUtil(CBinNode* x, int space);
    void print2D(CBinNode* x);
    void Medidor_espacios(CBinNode* x, CBinNode** p);
    void Arboldraw(CBinNode* x);
    void AVL(CBinNode* A, CBinNode* B, CBinNode* C, nodofila*D);
    void ver_AVL(CBinNode* x, CBinNode** p, CBinNode*** a, CBinNode**** D);
    //CBinNode* root;
    bool brep;
    Stack pila;
    
};

CBinTree::CBinTree()
{
    root = 0;
    brep = 0;
}

CBinTree::~CBinTree()
{
}

bool CBinTree::Find(int x, CBinNode**& p, Stack& arr)
{
    for (p = &root;
        *p && (*p)->value != x;
        p = &((*p)->nodes[(*p)->value < x])) {
        arr.push(*p);
        };
    return *p != 0;
}

bool CBinTree::Find_del(int x, CBinNode**& p, Stack& arr)
{
    for (p = &root;
        *p && (*p)->value != x;
        p = &((*p)->nodes[(*p)->value < x])) {
        arr.push(*p);
    };
    if ((*p)->value == x) { arr.push(*p); }
    return *p != 0;
}
int altura_AVL(CBinNode* nodin) {
    int avl = 0;
    if (nodin->nodes[0] == nullptr && nodin->nodes[1] == nullptr) {
    }
    else if (nodin->nodes[0] == nullptr && nodin->nodes[1] != nullptr) {
        avl = nodin->nodes[1]->profundidad - 0;
    }
    else if (nodin->nodes[0] != nullptr && nodin->nodes[1] == nullptr) {
        avl = 0 - nodin->nodes[0]->profundidad;
    }
    else {
        avl = nodin->nodes[1]->profundidad - nodin->nodes[0]->profundidad;
    }
    return avl;

}


bool CBinTree::Insert(int x)
{
    CBinNode** p;
    Stack Noditos;
    if (Find(x, p, ref(Noditos))) return 0;
    *p = new CBinNode(x);
    Noditos.push(*p);
    //actualizar alturas
    for (nodofila* i = Noditos.top; i; i = i->next) {
        int left = 0;
        int right = 0;
        if (i->nodo->nodes[0] == nullptr && i->nodo->nodes[1] != nullptr) {
            left = 0;
            right = i->nodo->nodes[1]->profundidad;
        }
        else if (i->nodo->nodes[0] != nullptr && i->nodo->nodes[1] == nullptr) {
            left = i->nodo->nodes[0]->profundidad;
            right = 0;
        }
        else if (i->nodo->nodes[0] == nullptr && i->nodo->nodes[1] == nullptr) {
            left = 0;
            right = 0;
        }
        else {
            left = i->nodo->nodes[0]->profundidad;
            right = i->nodo->nodes[1]->profundidad;
        }
        i->nodo->profundidad = max(left, right) + 1;
    }
    // ver si se hace el AVL
    if (Noditos.size() >= 3) {

        nodofila* A = Noditos.top;
        nodofila* B = A->next;
        nodofila* C = B->next;

        for (nodofila* i = C; C!=nullptr; A = B, B = C,C = C->next) {
            nodofila* D(nullptr);
            if (C->next == nullptr) {}
            else { D = C->next; }
            if (i = Noditos.top) {
                A->AVL_altura = altura_AVL(A->nodo);
                B->AVL_altura = altura_AVL(B->nodo);
                C->AVL_altura = altura_AVL(C->nodo);
            }
            else { C->AVL_altura = altura_AVL(C->nodo); }
            if (C->AVL_altura == 2 || C->AVL_altura == -2) {
                AVL(A->nodo, B->nodo, C->nodo, D);
                Profundidad(root);
                break;
            }
        }
    }
}


void CBinTree::ver_AVL(CBinNode* x, CBinNode** p, CBinNode*** a,CBinNode**** d) {
    if (!x) { return; }
    ver_AVL(x->nodes[0], &x, &p,&a);
    if ((*p) != (**a)) {
        Stack Noditos;
        Noditos.push(***d);
        Noditos.push(**a);
        Noditos.push(*p);
        Noditos.push(x);
        nodofila* A = Noditos.top;
        nodofila* B = A->next;
        nodofila* C = B->next;
        A->AVL_altura = altura_AVL(A->nodo);
        B->AVL_altura = altura_AVL(B->nodo);
        C->AVL_altura = altura_AVL(C->nodo);
        nodofila* D(nullptr);
        if (C->next == nullptr) {}
        else { D = C->next; }

        if (C->AVL_altura == 2 || C->AVL_altura == -2) {
            AVL(A->nodo, B->nodo, C->nodo,D);
            Profundidad(root);
            return;
        }
    }
    ver_AVL(x->nodes[1], &x, &p, &a);

}
bool CBinTree::Remove(int x)
{
    CBinNode** p;
    Stack Noditos;
    if (!Find_del(x, p, Noditos))
        return false;

    Noditos.pop();

    if ((*p)->nodes[0] && (*p)->nodes[1]) // Caso 2
    {
        CBinNode** q = Rep(p); // Corrección: Pasar la dirección de p a Rep
        (*p)->value = (*q)->value;
        p = q;
    }

    CBinNode* t = *p;
    *p = (*p)->nodes[(*p)->nodes[1] != nullptr];
    if (Noditos.top == nullptr) {}
    else { Noditos.top->nodo->profundidad -= 1; }

    // Actualizar alturas
    for (nodofila* i = Noditos.top; i; i = i->next) {
        int left = 0;
        int right = 0;

        if (i->nodo->nodes[0] == nullptr && i->nodo->nodes[1] != nullptr) {
            left = 0;
            right = i->nodo->nodes[1]->profundidad;
        }
        else if (i->nodo->nodes[0] != nullptr && i->nodo->nodes[1] == nullptr) {
            left = i->nodo->nodes[0]->profundidad;
            right = 0;
        }
        else if (i->nodo->nodes[0] == nullptr && i->nodo->nodes[1] == nullptr) {
            left = 0;
            right = 0;
        }
        else {
            left = i->nodo->nodes[0]->profundidad;
            right = i->nodo->nodes[1]->profundidad;
        }

        i->nodo->profundidad = std::max(left, right) + 1;
    }

    CBinNode** ptr = &root;
    CBinNode*** ptr2 = &ptr;
    ver_AVL(root, &root, &ptr,&ptr2);
    delete t;
    return true;
}

CBinNode** CBinTree::Rep(CBinNode** p)
{
    CBinNode** q = p;
    int b = rand() % 2;
    for (q = &((*q)->nodes[b]); *q &&(*q)->nodes[!b] !=nullptr; q = &((*q)->nodes[!b])) { cout << " "; };
    return q;
}


void CBinTree::InOrder(CBinNode* x)
{
    if (!x) return;
    InOrder(x->nodes[0]);
    cout << x->value << " -> Profundidad: " << x->profundidad << endl;
    InOrder(x->nodes[1]); 

}




void CBinTree::PostOrder(CBinNode* x)
{
    if (!x) return;
    cout << x->value << " -> Profundidad: " << x->profundidad << endl;
    PostOrder(x->nodes[0]);
    PostOrder(x->nodes[1]);

}



void CBinTree::PreOrder(CBinNode* x)
{
    if (!x) return;
    PreOrder(x->nodes[0]);
    PreOrder(x->nodes[1]);
    cout << x->value << " -> Profundidad: " << x->profundidad << endl;

}




void CBinTree::Inverse(CBinNode* x)
{
    if (!x) return;
    InOrder(x->nodes[1]);
    cout << x->value << " -> Profundidad: " << x->profundidad << endl;
    InOrder(x->nodes[0]);

}




void CBinTree::InOrder_manual(CBinNode* x) {
    pila.push(x);
    while (pila.top) {
        if (pila.top->estado == 0) {
            if (pila.top->nodo->nodes[0] != nullptr) {
                pila.top->estado += 1;
                pila.push(pila.top->nodo->nodes[0]);
            }
            else { pila.top->estado += 1; }
        }
        else if (pila.top->estado == 1) {
            cout << pila.top->nodo->value << " -> Profundidad: " << pila.top->nodo->profundidad <<endl;
            pila.top->estado += 1;
        }
        else if (pila.top->estado == 2) {
            if (pila.top->nodo->nodes[1] != nullptr) {
                pila.top->estado += 1;
                pila.push(pila.top->nodo->nodes[1]);
            }
            else { pila.top->estado += 1; }
        }
        else { pila.pop(); }
    }
}


bool ver_padre(CBinNode* C, CBinNode* D) {
    if (D->nodes[0] == C) { return false; }
    else { return true; }
}


void CBinTree::AVL(CBinNode* A, CBinNode* B, CBinNode* C, nodofila* D) {

    if (B->nodes[0] == A && C->nodes[0] == B) {
        C->nodes[0] = B->nodes[1];
        B->nodes[1] = C;
        if (D!=nullptr) D->nodo->nodes[ver_padre(C, D->nodo)] = B;
        else { root = B; }
    }
    else if (B->nodes[1] == A && C->nodes[1] == B) {
        C->nodes[1] = B->nodes[0];
        B->nodes[0] = C;
        if (D!= nullptr) D->nodo->nodes[ver_padre(C, D->nodo)] = B;
        else { root = B; }
    }
    else if (B->nodes[0] == A && C->nodes[1] == B) {
        C->nodes[1] = A->nodes[0];
        B->nodes[0] = A->nodes[1];
        A->nodes[0] = C;
        A->nodes[1] = B;
        if (D!= nullptr) D->nodo->nodes[ver_padre(C, D->nodo)]= A;
        else { root = A; }
    }
    else if (B->nodes[1] == A && C->nodes[0] == B) {
        B->nodes[1] = A->nodes[0];
        C->nodes[0] = A->nodes[1];
        A->nodes[0] = B;
        A->nodes[1] = C;
        if (D!= nullptr) D->nodo->nodes[ver_padre(C, D->nodo)] = A;
        else { root = A; }
    }
    

}/*


void CBinTree::Medidor_espacios(CBinNode* x, CBinNode** p) {
    if (!x) { return; }
    if (x==root) {
        x->espacio_izq = x->espacio_der = x->profundidad ;
    }
    else {
    x->espacio_izq = x->profundidad - 1;
    x->espacio_der = (*p)->profundidad - x->profundidad ;
   /* x->espacio_izq = (x->profundidad) + 1;
    x->espacio_der = (*p)->profundidad/ x->profundidad;*//*
    }
    Medidor_espacios(x->nodes[0],&x);
    Medidor_espacios(x->nodes[1],&x);
    return;
}   */



/*
void CBinTree::Arboldraw(CBinNode* x) {
    Queue cola_print;
    int salir=0;
    int div = 0;
    int relleno = 0;
    int cont = 1;
    int cont2 = x->profundidad*3;
    int cont3 = cont;
    cola_print.push(x);
    while (cola_print.head) {
        cola_print.temp = cola_print.head;
        for (int i = 1; i <= cont3; i++) {
            if (cola_print.temp->nodo == nullptr) {
                CBinNode* temp = nullptr;
                cola_print.push(temp);
                cola_print.push(temp);
            }
            else if (cola_print.temp->nodo->nodes[0] == nullptr && cola_print.temp->nodo->nodes[1] != nullptr) {
                CBinNode* temp = nullptr;
                cola_print.push(temp);
                cola_print.push(cola_print.temp->nodo->nodes[1]);
                relleno += 2;
            }
            else if (cola_print.temp->nodo->nodes[1] == nullptr && cola_print.temp->nodo->nodes[0] != nullptr) {
                CBinNode* temp = nullptr;
                cola_print.push(cola_print.temp->nodo->nodes[0]);
                cola_print.push(temp);

            }
            else if (cola_print.temp->nodo->nodes[0] == nullptr && cola_print.temp->nodo->nodes[1] == nullptr) {
                CBinNode* temp = nullptr;
                cola_print.push(temp);
                cola_print.push(temp);
                relleno += 6;
            }
            else {
                cola_print.push(cola_print.temp->nodo->nodes[0]);
                cola_print.push(cola_print.temp->nodo->nodes[1]);
            }
            cola_print.temp = cola_print.temp->next;
        }
        cont = cont3;
        cont3 *= 2;
        for (int j = 0; j <= cont2; j++) { cout << " "; }
        for (int i = 1; i <= cont; i++) {
            int relleno2 = relleno;
            if (cola_print.head->nodo == nullptr) {
                for (nodofila* temp = cola_print.head; temp != cola_print.tail->next; temp = temp->next) {
                    if (temp->nodo != nullptr) div += 1;
                }
                for (int i = 0; i <= relleno/div; i++) { cout << " "; }
                while (cola_print.head->nodo == nullptr && cont >= 0) { cola_print.pop(); cont -= 1; }

                if (cola_print.head->nodo != nullptr) {
                    for (int i = 0; i <= cola_print.head->nodo->espacio_izq; i++)  cout << " ";
                    cout << cola_print.head->nodo->value;
                    for (int i = 0; i <= cola_print.head->nodo->espacio_der; i++)  cout << "  ";
                    cola_print.pop();
                    cont -= 1;
                }
                
            }
            else {
                for (int i = 0; i <= cola_print.head->nodo->espacio_izq; i++)  cout <<  " ";
                cout << cola_print.head->nodo->value;
                for (int i = 0; i <= cola_print.head->nodo->espacio_der; i++)  cout << " ";
                cola_print.pop();
                }
                
        }
        for (nodofila* temp = cola_print.head; temp != cola_print.tail->next; temp = temp->next) {
            if (temp->nodo != nullptr) salir += 1;
        }
        if (salir == 0) { cola_print.head = nullptr; }
        cout << endl;
        salir = 0;
        cont *= 2;
        cont2 -= 4;
        div = 0;

    }


}*/



void CBinTree::print2DUtil(CBinNode* x, int space)
{
    // Base case
    if (x == NULL)
        return;

    // Increase distance between levels
    space += COUNT;

    // Process right child first
    print2DUtil(x->nodes[1], space);

    // Print current node after space
    // count
    cout << endl;
    for (int i = COUNT; i < space; i++)
        cout << " ";
    cout << x->value << "\n";

    // Process left child
    print2DUtil(x->nodes[0], space);
};



// Wrapper over print2DUtil()
void CBinTree::print2D(CBinNode* x)
{
    // Pass initial space count as 0
    print2DUtil(x, 0);
}


int CBinTree::Profundidad(CBinNode* x) {

    if (!x) { return 0; }
    int left = Profundidad(x->nodes[0]);
    int right = Profundidad(x->nodes[1]);
    x->profundidad = max(left, right) + 1;
    return max(left, right) + 1;
};



void CBinTree::Print()
{
    CBinNode** padre=nullptr;
    //Profundidad(root);
    //InOrder(root); ENDL;
   //PostOrder(root); ENDL;
   //Medidor_espacios(root,padre);
   //Arboldraw(root);
   //PreOrder(root); ENDL;
   //Inverse(root); ENDL;
  // printTree(400, 50, root, 1, window, font);
   InOrder_manual(root);
   print2D(root);
}

void CBinTree::printTree(int x, int y, CBinNode* node, int index, sf::RenderWindow& window, sf::Font font)
{
    if (!node)
        return;

    sf::CircleShape circle(15);
    circle.setFillColor(sf::Color(0, 255, 0)); 
    circle.setPosition(x-15, y-3);
    sf::Text text(std::to_string(node->value), font);
    text.setCharacterSize(15);
    text.setFillColor(sf::Color::White);
    text.setPosition(x , y);

    int left = 2 * index + 50;
    int right = 2 * index + 100;

    printTree((x - y / (index + 1)-20), y + 70, node->nodes[0], left, window,font);
    printTree((x + y / (index + 1)+40), y + 70, node->nodes[1], right, window, font);

    if (node->nodes[0] != 0) {
        sf::Vertex line1[] =
        {
            sf::Vertex(sf::Vector2f(x, y)),
            sf::Vertex(sf::Vector2f((x - y / (index + 1))-20, y + 70))
        };
        window.draw(line1, 2, sf::Lines);
    }

    if (node->nodes[1] != 0) {
        sf::Vertex line2[] =
        {
            sf::Vertex(sf::Vector2f(x, y)),
            sf::Vertex(sf::Vector2f((x + y / (index + 1))+40, y + 70))
        };
        window.draw(line2, 2, sf::Lines);
    }

    window.draw(circle);
    window.draw(text);
}



int main()
{
    // Crear una ventana de SFML
    sf::RenderWindow window(sf::VideoMode(800, 600), "Mi Árbol Binario");

    // Cargar una fuente
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
        return EXIT_FAILURE;
    
     

    CBinTree t;
    t.Insert(50);
    t.Insert(60);
    t.Insert(70);
    t.Insert(90);
    t.Insert(95);
    t.Insert(65);
    t.Insert(68);
    t.Insert(66);
    t.Insert(67);
    t.Insert(69);
    t.Insert(64);
    t.Insert(61);
    t.Remove(70);
    t.Remove(90);
    t.Remove(66);
    // Bucle principal
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        // Llamar a la función printTree
        //t.Print();
        t.printTree(400, 50, t.root, 1, window, font);
        //t.Print(window, font);

        window.display();
    }

    return 0;
}


