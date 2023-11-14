#include <string>
#include <iostream>

using namespace std;

class Team{
    int wins;
    int losses;
    string name;
  public:
      int index_max = -1;
      int index_min = -1;
      Team(int wins, int losses, string name) {
        this->wins = wins;
        this->losses = losses;
        this->name = name;
      }
      Team(){}
      int getWins() const;
      int getLosses() const;
      string getName() const;
};

class TeamPQ {
  public:
      Team* *MaxHeap = new Team*[1];
      Team* *MinHeap = new Team*[1];
      Team *teams = new Team[1];
      int num_team = 0;
      int current_size = 1;
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
      void clean() {
        delete[] MaxHeap;
        delete[] MinHeap;
        delete[] teams;
        num_team = 0;
        current_size = 0;
      }
};



int lchild(int i) {
    return 2*i+1;
}

int rchild(int i) {
    return 2*i+2;
}

int parent(int i) {
    return (i-1)/2;
}

void TeamPQ::fixup_max(int i) {
    while(parent(i) >=0 && (MaxHeap[parent(i)]->getWins() < MaxHeap[i]->getWins())) {
        Team* temp = MaxHeap[i];
        MaxHeap[i] = MaxHeap[parent(i)];
        MaxHeap[parent(i)] = temp;
        swap(MaxHeap[i]->index_max,MaxHeap[parent(i)]->index_max);
        i = parent(i);
    }
}

void TeamPQ::fixup_min(int i) {
    while(parent(i) >= 0 && (MinHeap[parent(i)]->getLosses() > MinHeap[i]->getLosses())) {
        Team* temp = MinHeap[i];
        MinHeap[i] = MinHeap[parent(i)];
        MinHeap[parent(i)] = temp;
        swap(MinHeap[i]->index_min,MinHeap[parent(i)]->index_min);
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
    insertToMin(&teams[num_team - 1]);
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
        swap(MaxHeap[k],MaxHeap[j]);
        swap(MaxHeap[k]->index_max,MaxHeap[j]->index_max);
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
        swap(MinHeap[k],MinHeap[j]);
        swap(MinHeap[k]->index_min,MinHeap[j]->index_min);
        k = j;
    }
}

void TeamPQ::removeMaxWins() {
    int i = num_team - 1;
    swap(MaxHeap[0], MaxHeap[i]);
    swap(MaxHeap[i]->index_max, MaxHeap[0]->index_max);
    swap(teams[i], *MaxHeap[i]);
    swap(MinHeap[teams[i].index_min], MinHeap[MaxHeap[i]->index_min]);
    swap(MaxHeap[teams[i].index_max], MaxHeap[i]);
    num_team--;
    int j = teams[i].index_min;
    swap(MinHeap[j], MinHeap[i]);
    swap(MinHeap[i]->index_min,MinHeap[j]->index_min);
    fixdown_max(num_team,0);
    fixdown_min(num_team,j);
}

void TeamPQ::removeMinLosses() {
    int i = num_team - 1;
    swap(MinHeap[0], MinHeap[i]);
    swap(MinHeap[i]->index_min, MinHeap[0]->index_min);
    swap(teams[i], *MinHeap[i]);
    swap(MaxHeap[teams[i].index_max], MaxHeap[MinHeap[i]->index_max]);
    swap(MinHeap[teams[i].index_min], MinHeap[i]);
    num_team--;
    int j = teams[i].index_max;
    swap(MaxHeap[j], MaxHeap[i]);
    swap(MaxHeap[i]->index_max, MaxHeap[j]->index_max);
    fixdown_min(num_team,0);
    fixdown_max(num_team,j);
}

void TeamPQ::removeAllK(int k) {
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

string Team::getName() const {
    return name;
}


int main() {
    string command = "";
    cin >> command;
    TeamPQ PQ;
    while(1) {
        if(command == "r") {
            PQ.clean();
            PQ.MaxHeap = new Team*[1];
            PQ.MinHeap = new Team*[1];
            PQ.teams = new Team[1];
            PQ.num_team = 0;
            PQ.current_size = 1;
        } else if(command == "x") {
            PQ.clean();
            return 0;
        } else if(command == "i") {
            int wins, losses;
            string name;
            cin >> wins >> losses >> name;
            PQ.insert(Team(wins, losses, name));
        } else if(command == "pw") {
            cout << PQ.findMaxWins().getName() << endl;
        } else if(command == "pl") {
            cout << PQ.findMinLosses().getName() << endl;
        } else if(command == "rw") {
            if(PQ.num_team == 0) continue;
            PQ.removeMaxWins();
        } else if(command == "rl") {
            if(PQ.num_team == 0) continue;
            PQ.removeMinLosses();
        } else if(command == "ra") {
            int k;
            cin >> k;
            PQ.removeAllK(k);
        } else {
            cerr << "Invalid command" << endl;
            return 0;
        }
        cin >> command;
    }

    return 0;
}
