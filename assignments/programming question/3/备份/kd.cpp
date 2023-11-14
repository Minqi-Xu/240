#include <iostream>
#include <random>
#include <climits>

using namespace std;

struct point{           // point structure
    int x, y;
};

struct treenode{
    point p;            // points which using to split OR if num_point == 1, the point on the leaf.
    int num_point;      // number of points in the corresponding range
    int direction;      // 0 means separate the range horizontally, 1 means separate the range vertically
    int l,r;            // l,r are the boundary of these its points that are in the array
    treenode* lt;       // child which less than the p in corresponding direction
    treenode* geq;      // child which greater or equal to the p in corresponding direction
    int min_x, max_x, min_y, max_y;
};

// Initialize a node of tree including delete its children
void initialize(treenode* tree) {
    if(tree == NULL) return;
    if(tree->num_point == 1)    return;
    initialize(tree->lt);
    initialize(tree->geq);
    delete tree->lt;
    delete tree->geq;
}


// Two partition algorithms (Hoare) based on x and y
int x_partition(point* arr, int l, int r, int p) {
    swap(arr[r], arr[p]);
    int i = l - 1;
    int j = r;
    int v = arr[r].x;
    while(true) {
        do {
            i++;
        } while(arr[i].x < v);
        do {
            j--;
        } while(j >= i && arr[j].x > v);
        if(i >= j)  break;
        else    swap(arr[i], arr[j]);
    }
    swap(arr[r], arr[i]);
    return i;
}

int y_partition(point* arr, int l, int r, int p) {
    swap(arr[r], arr[p]);
    int i = l - 1;
    int j = r;
    int v = arr[r].y;
    while(true) {
        do {
            i++;
        } while(arr[i].y < v);
        do {
            j--;
        } while(j >= i && arr[j].y > v);
        if(i >= j)  break;
        else    swap(arr[i], arr[j]);
    }
    swap(arr[r], arr[i]);
    return i;
}

// Two quick selection algorithm using random pivoting based on x and y
int x_quick_select(point* arr, int l, int r, int k) {
    int p = rand() % (r-l+1);
    int i = x_partition(arr, l, r, l+p);
    if(i == l + k)        return i;
    else if(i > l + k)    return x_quick_select(arr, l, i-1, k);
    else                  return x_quick_select(arr, i+1, r, l+k-(i+1));
}

int y_quick_select(point* arr, int l, int r, int k) {

    int p = rand() % (r-l+1);
    int i = y_partition(arr, l, r, l+p);
    if(i == l + k)      return i;
    else if(i > l + k)  return y_quick_select(arr, l, i-1, k);
    else                return y_quick_select(arr, i+1, r, l+k-(i+1));
}

void build_x(treenode*, int, int, point*, int, int, int, int);
void build_y(treenode*, int, int, point*, int, int, int, int);

void build_x(treenode* n, int l, int r, point* arr, int xmin, int xmax, int ymin, int ymax) {       // build a node split by x
    if(r-l == 0) {
        n->p = arr[l];
        n->num_point = 1;
        n->direction = 1;
        n->l = l;
        n->r = r;
        return;
    }
    int i = x_quick_select(arr, l, r, (r-l+1)/2);
    n->p = arr[i];
    n->num_point = l - r + 1;
    n->direction = 1;
    n->l = l;
    n->r = r;
    n->max_x = xmax;
    n->max_y = ymax;
    n->min_x = xmin;
    n->min_y = ymin;
    x_partition(arr, l, r, i);
    n->lt = new treenode;
    n->geq = new treenode;
    build_y(n->lt, l, i-1, arr, xmin, n->p.x-1, ymin, ymax);
    build_y(n->geq, i, r, arr, n->p.x, xmax, ymin, ymax);
}

void build_y(treenode* n, int l, int r, point* arr, int xmin, int xmax, int ymin, int ymax) {       // build a node split by y
    if(r-l == 0) {
        n->p = arr[l];
        n->num_point = 1;
        n->direction = 0;
        n->l = l;
        n->r = r;
        return;
    }
    int i = y_quick_select(arr, l, r, (r-l+1)/2);
    n->p = arr[i];
    n->num_point = l - r + 1;
    n->direction = 0;
    n->l = l;
    n->r = r;
    n->max_x = xmax;
    n->max_y = ymax;
    n->min_x = xmin;
    n->min_y = ymin;
    y_partition(arr, l, r, i);
    n->lt = new treenode;
    n->geq = new treenode;
    build_x(n->lt, l, i-1, arr, xmin, xmax, ymin, n->p.y-1);
    build_x(n->geq, i, r, arr, xmin, xmax, n->p.y, ymax);
}

void tranverse(treenode* n) {       // preorder tranversal and print the p in each node
    if(n->num_point == 1) {
        cout << n->p.x << " " << n->p.y << endl;
        return;
    }
    cout << n->p.x << " " << n->p.y << endl;
//    cout << "Not leaf: " << n->p.x << " " << n->p.y << ": (" << n->min_x << ", " << n->max_x << ") * (" << n->min_y << ", " << n->max_y << ")" << endl;
    tranverse(n->lt);
    tranverse(n->geq);
}

void print_points(treenode* n) {        // preorder tranversal and print all the leaves (points)
    if(n->num_point == 1) {
        cout << n->p.x << " " << n->p.y << endl;
        return;
    }
    tranverse(n->lt);
    tranverse(n->geq);

}

// kdTree::RangeSearch
void range_search(treenode* r, int xmin, int xmax, int ymin, int ymax) {
    if(r->num_point == 1) {
        if(r->p.x >= xmin && r->p.x <= xmax && r->p.y >= ymin && r->p.y <= ymax) {
            cout << r->p.x << " " << r->p.y << endl;
            return;
        } else {
            return;
        }
    }
    if(xmin <= r->min_x && xmax >= r->max_x && ymin <= r->min_y && ymax >= r->max_y) {
        print_points(r);
        return;
    }
    if(xmin > r->max_x || xmax < r->min_x || ymin > r->max_y || ymax < r->min_y)
    {
        return;
    }

    range_search(r->lt, xmin, xmax, ymin, ymax);
    range_search(r->geq, xmin, xmax, ymin, ymax);
}

int main() {
    point *arr = NULL;
    treenode* tree = NULL;
    while(1) {
        string command;
        cin >> command;
        if(command == "r") {
            if(arr) delete[] arr;
            arr = NULL;
            initialize(tree);
            if(tree) delete tree;
            tree = new treenode;
        } else if (command == "x") {
            if(arr)     delete[] arr;
            arr = NULL;
            initialize(tree);
            if(tree) delete tree;
            break;
        } else if (command == "ikd") {
            if(arr) {
                cerr << "The current kd-tree already exists!" << endl;
                initialize(tree);
                if(tree)    delete tree;
                delete[] arr;
                return 1;
            }
            int n;
            cin >> n;
            if(n == 0)  continue;
            arr = new point[n];
            for(int i = 0; i < n; i++) {
                int x,y;
                cin >> x >> y;
                point p{x,y};
                arr[i] = p;
            }
            build_x(tree,0,n-1,arr, INT_MIN, INT_MAX, INT_MIN, INT_MAX);

        } else if (command == "pkd") {
            if(arr == NULL) continue;
            tranverse(tree);

        } else if (command == "skd") {
            int xmin, xmax, ymin, ymax;
            cin >> xmin >> xmax >> ymin >> ymax;
            range_search(tree, xmin, xmax-1, ymin, ymax-1);

        } else {
            cerr << "undefine command!" << endl;
            return 1;
        }
    }

    return 0;
}
