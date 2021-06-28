
#define max_points 1


typedef struct vec2{
    double x, y;
}Vec2;



typedef struct particle{
    int number;
    float mass;
    float radius;
    Vec2 pos;
    Vec2 vel;
} Particle;



typedef struct listnode{  
    Particle *p;
    struct listnode *next;
}ListNode;



typedef struct list{
    int size;
    ListNode *root;
}List;



typedef struct quadnode {
    Vec2 Origin;
    Vec2 width_height;
    int dividido;
    List part_list;
    struct quadnode *downleft, *downright, *upleft, *upright;
} QuadNode;



typedef struct quadtree{
    float size;
    QuadNode *Raiz;
    List *List_Part;
} Quad;

Particle *start_Part(int, float , float , Vec2 , Vec2 );

void destroy_Particle(Particle *);

ListNode *start_ListNode(Particle *);

List *start_List();

int insert_List(Particle *, List *);

ListNode *search_ListNode(Particle ,List *);

void destroy_ListNode(ListNode *);

int pop_List(Particle , List *);

List *copy_List(List *);

void destroy_List(List *);

QuadNode *start_QuadNode(Vec2 , Vec2 );

Quad *start_Quad(QuadNode *, List *);

void check_Pos(Particle *, QuadNode *);

void quadNode_Destroy(QuadNode *);

void quad_Check(QuadNode *);

void destroy_Quad(Quad *);



