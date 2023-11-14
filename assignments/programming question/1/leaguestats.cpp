#include <string>
#include <iostream>
#include <iomanip>

//using namespace std;

class Team{
    int wins;
    int losses;
    std::string name;
  public:
      int index_max = -1;   // Index_max stores the index number of current element in the MaxHeap
      int index_min = -1;   // Index_min stores the index number of current element in the MinHeap
      Team(int wins, int losses, std::string name) {    // constructor
        this->wins = wins;
        this->losses = losses;
        this->name = name;
      }
      Team(){}
      int getWins() const;
      int getLosses() const;
      std::string getName() const;
};

class TeamPQ {
  public:
      Team* *MaxHeap = new Team*[1];    // MaxHeap[] is the array that stores some pointer of Team, and is in the shape of maxheap
      Team* *MinHeap = new Team*[1];    // MinHeap[] is the array that stores some pointer of Team, and is in the shape of minheap
      Team *teams = new Team[1];    // teams[] is the array that contains all teams unordered.
      // Since we connect three arrays with pointer, and the variables in Team, when we change any of the array(i.e. exchange some entries
      // we also need to consider the changing in other two arrays, which leads to multiple swap appear consequtively)
      int num_team = 0;
      int current_size = 1;
      void reset() {        // initialize the variables and arrays.
            MaxHeap = new Team*[1];
            MinHeap = new Team*[1];
            teams = new Team[1];
            num_team = 0;
            current_size = 1;
      }
      void insert(const Team &t);   //O(log n) time
      const Team &findMaxWins() const;    // O(1) time
      const Team &findMinLosses() const;    // O(1) time
      void removeMaxWins();   // O(log n) time
      void removeMinLosses();    // O(log n) time
      void removeAllK(int k);    // ??? time
      void fixup_max(int i);
      void fixup_min(int i);
      void insertToMax(Team *t);
      void insertToMin(Team *t);
      void fixdown_max(int num_team, int k);
      void fixdown_min(int num_team, int k);
      void clean() {    // free the memory and initialize the variables.
        delete[] MaxHeap;
        delete[] MinHeap;
        delete[] teams;
        num_team = 0;
        current_size = 0;
      }

      void debug() {        // this function is for debug usage
        std::cout << "Debug!" << std::endl << "MAXHEAP:" << std::endl;
        for(int i = 0; i < num_team; ++i) {
            std::cout << MaxHeap[i]->getName() << std::endl;
        }
        std::cout << "MINHEAP:" << std::endl;
        for(int i = 0; i < num_team; ++i) {
            std::cout << MinHeap[i]->getName() << std::endl;
        }
        std::cout << "TEAMS:" << std::endl;
        for(int i = 0; i < num_team; ++i) {
            std::cout << teams[i].getName() << " max_index: " << teams[i].index_max << " min_index: " << teams[i].index_min << std::endl;
        }
      }
};

int lchild(int i) {     // calculate the index of left child
    return 2*i+1;
}

int rchild(int i) {     // calculate the index of right child
    return 2*i+2;
}

int parent(int i) {     // calculate the index of parent
    return (i-1)/2;
}

void TeamPQ::fixup_max(int i) {
    while(parent(i) >=0 && (MaxHeap[parent(i)]->getWins() < MaxHeap[i]->getWins())) {
        Team* temp = MaxHeap[i];
        MaxHeap[i] = MaxHeap[parent(i)];
        MaxHeap[parent(i)] = temp;
        std::swap(MaxHeap[i]->index_max,MaxHeap[parent(i)]->index_max);
        i = parent(i);
    }
}

void TeamPQ::fixup_min(int i) {
    while(parent(i) >= 0 && (MinHeap[parent(i)]->getLosses() > MinHeap[i]->getLosses())) {
        Team* temp = MinHeap[i];
        MinHeap[i] = MinHeap[parent(i)];
        MinHeap[parent(i)] = temp;
        std::swap(MinHeap[i]->index_min,MinHeap[parent(i)]->index_min);
        i = parent(i);
    }
}

void TeamPQ::insertToMax(Team *t) {
    int i = num_team - 1;
    MaxHeap[i] = t;
    t->index_max = i;
    fixup_max(i);
}

void TeamPQ::insertToMin(Team *t) {
    int i = num_team - 1;
    MinHeap[i] = t;
    t->index_min = i;
    fixup_min(i);
}

void TeamPQ::insert(const Team &t) {
    num_team++;
    if(num_team > current_size) {
        Team *temp = new Team[current_size * 2];
        for(int i = 0; i < current_size; ++i) {
            temp[i] = teams[i];
            MaxHeap[teams[i].index_max] = &temp[i];
            MinHeap[teams[i].index_min] = &temp[i];
        }
        delete[] teams;
        teams = temp;

        Team* *tempHeap = new Team*[current_size * 2];
        for(int i = 0; i < current_size; ++i) {
            tempHeap[i] = MaxHeap[i];
        }
        delete[] MaxHeap;
        MaxHeap = tempHeap;

        tempHeap = new Team*[current_size * 2];
        for(int i = 0; i < current_size; ++i) {
            tempHeap[i] = MinHeap[i];
        }
        delete[] MinHeap;
        MinHeap = tempHeap;

        current_size *= 2;
    }
    teams[num_team - 1] = t;
    insertToMax(&teams[num_team - 1]);
    insertToMin(&teams[num_team - 1]);      // both priority queue need to be maintained.
}

const Team& TeamPQ::findMaxWins() const {
    return *MaxHeap[0];
}

const Team& TeamPQ::findMinLosses() const {
    return *MinHeap[0];
}

void TeamPQ::fixdown_max(int num_team, int k) {
    while(lchild(k) < num_team) {
        int j = lchild(k);
        if((j < num_team - 1) && MaxHeap[j+1]->getWins() > MaxHeap[j]->getWins())
            j++;
        if(MaxHeap[k]->getWins() >= MaxHeap[j]->getWins())
            break;
        std::swap(MaxHeap[k],MaxHeap[j]);
        std::swap(MaxHeap[k]->index_max,MaxHeap[j]->index_max);
        k = j;
    }
}

void TeamPQ::fixdown_min(int num_team, int k) {
    while(lchild(k) < num_team) {
        int j = lchild(k);
        if((j < num_team - 1) && MinHeap[j+1]->getLosses() < MinHeap[j]->getLosses())
            j++;
        if(MinHeap[k]->getLosses() <= MaxHeap[j]->getLosses())
            break;
        std::swap(MinHeap[k],MinHeap[j]);
        std::swap(MinHeap[k]->index_min,MinHeap[j]->index_min);
        k = j;
    }
}

void TeamPQ::removeMaxWins() {
    int i = num_team - 1;
    Team* ptr1 = &teams[i];
    Team* ptr2 = MaxHeap[0];
/*    for(int ff = 0; ff < num_team ; ++ff) {
        cout << "TEAMS:" << endl;
        cout << teams[ff].getName() << endl;
    }*/   //debug usage
    std::swap(MinHeap[ptr1->index_min], MinHeap[ptr2->index_min]);
    std::swap(MaxHeap[ptr1->index_max], MaxHeap[ptr2->index_max]);
//    swap(*ptr1, *ptr2);
    Team temp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = temp;       // we first swap the Team in teams array, with the corresponding adjustment on another two array.
/*    for(int ff = 0; ff < num_team ; ++ff) {
        cout << "TEAMS:" << endl;
        cout << teams[ff].getName() << endl;
    }*/    //debug usage
    std::swap(MaxHeap[0], MaxHeap[i]);
    std::swap(MaxHeap[i]->index_max, MaxHeap[0]->index_max);    // we swap the first and the last entry in the maxheap
    num_team--;

    int j = teams[i].index_min;
    std::swap(MinHeap[j], MinHeap[i]);
    std::swap(MinHeap[i]->index_min,MinHeap[j]->index_min);     // adjustment on the other array(PQ)
    fixdown_max(num_team,0);                                    // adjustment to fit the order property.
    fixup_min(j);                                               // adjustment to fit the order property.
    fixdown_min(num_team,j);                                    // adjustment to fit the order property.
}

void TeamPQ::removeMinLosses() {        // similar to the previous function
    int i = num_team - 1;
    Team* ptr1 = &teams[i];
    Team* ptr2 = MinHeap[0];

    std::swap(MinHeap[ptr1->index_min], MinHeap[ptr2->index_min]);
    std::swap(MaxHeap[ptr1->index_max], MaxHeap[ptr2->index_max]);
    std::swap(MinHeap[0], MinHeap[i]);
    std::swap(MinHeap[i]->index_min, MinHeap[0]->index_min);
//    swap(*ptr1, *ptr2);
    Team temp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = temp;
    num_team--;

    int j = teams[i].index_max;
    std::swap(MaxHeap[j], MaxHeap[i]);
    std::swap(MaxHeap[i]->index_max, MaxHeap[j]->index_max);
    fixdown_min(num_team,0);
    fixup_max(j);
    fixdown_max(num_team,j);
}

void TeamPQ::removeAllK(int k) {        // repeately remove the first entry of the queue until we get one less than k
    while(MaxHeap[0]->getWins() >= k && num_team > 0) {
        removeMaxWins();
    }
}

int Team::getWins() const {
    return wins;
}

int Team::getLosses() const {
    return losses;
}

std::string Team::getName() const {
    return name;
}


int main() {
    std::string command = "";
    std::cin >> command;
    TeamPQ PQ;
    while(1) {
        if(command == "r") {
            PQ.clean();
            PQ.reset();
        } else if(command == "x") {
            PQ.clean();
            return 0;
        } else if(command == "i") {
            int wins, losses;
            std::string name;
            std::cin >> wins >> losses >> name;
            PQ.insert(Team(wins, losses, name));
        } else if(command == "pw") {
            std::cout << PQ.findMaxWins().getName() << std::endl;
//            PQ.debug();
        } else if(command == "pl") {
            std::cout << PQ.findMinLosses().getName() << std::endl;
 //           PQ.debug();
        } else if(command == "rw") {
            if(PQ.num_team == 0) continue;
            PQ.removeMaxWins();
        } else if(command == "rl") {
            if(PQ.num_team == 0) continue;
            PQ.removeMinLosses();
        } else if(command == "ra") {
            int k;
            std::cin >> k;
            PQ.removeAllK(k);
        } else {
            std::cerr << "Invalid command" << std::endl;
            return 0;
        }
        std::cin >> command;
    }

    return 0;
}
