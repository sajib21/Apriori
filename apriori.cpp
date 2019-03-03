#include<bits/stdc++.h>

#define vi      vector<int>
#define vvi     vector<vi>
#define pb(x)   push_back(x)
#define si      set<int>
#define all(x)  x.begin(), x.end()

#define LIM     30000
#define MAX     120
#define MIN_SUP_THRES 50

using namespace std;

int MIN_SUP_COUNT_THRES;

vvi database;
si items;
int numberOfTrans, numberOfItem;
vector<vvi>L;

set<vi>ase;

int MAXI = 0;

void printDatabase() {
    cout << "DATABASE: " << endl;
    for(int i=0; i<database.size(); i++) {
        cout << "T" << i << " : " ;
        for(auto it : database[i]) cout << it << ' ';
        cout << endl;
    }
    cout << endl;
}

struct info {
    int IDS;
    int id[LIM];
    int mark[LIM];
    int root;
//    int nxt[LIM][MAX];
    vvi nxt;
    int numberofCandidates;

    info() {
        numberofCandidates = 0;
        root = 1;
        IDS = 1;
        nxt.clear();
        vi temp;
        for(int i=0; i<=MAX; i++) temp.pb(0);
        nxt.pb(temp);
        nxt.pb(temp);
//        memset(nxt, 0, sizeof nxt);
        memset(id, 0, sizeof id);
        memset(mark, 0, sizeof mark);
    }
    clear() {
        numberofCandidates = 0;
        root = 1;
        IDS = 1;
        nxt.clear();
        vi temp;
        for(int i=0; i<=MAX; i++) temp.pb(0);
        nxt.pb(temp);
        nxt.pb(temp);
//        memset(nxt, 0, sizeof nxt);
        memset(id, 0, sizeof id);
        memset(mark, 0, sizeof mark);
    }

    void add(vi candidate, int indx) {
        int cur = root;
        for(auto item : candidate) {
//            assert(IDS < LIM-5);

//            if(nxt.size() <= cur)

            if(!nxt[cur][item]) {
                nxt[cur][item] = ++IDS;
                vi temp;
                for(int i=0; i<=MAX; i++) temp.pb(0);
                nxt.pb(temp);
            }
            cur = nxt[cur][item];
        }
        numberofCandidates++;
        mark[cur] = 1;
        id[cur] = indx;
//        cout << "CURRENT MAX " << IDS << endl;
        MAXI = max(MAXI, IDS);
    }

    vi datasetTest(vvi &database) {
        vi occurance;
        for(int i=0; i<numberofCandidates; i++) occurance.pb(0);
        int in = 0;
        for(auto line : database) {
            dfs(root, 0, line, occurance);
        }
        return occurance;
    }

    void dfs(int u, int in, vi &line, vi &occurance, int con = 0) {
        if(mark[u] && !con) occurance[ id[u] ]++;

        if(in == line.size()) return;

        int vval = line[in];
        if(nxt[u][vval]) dfs(nxt[u][vval], in+1, line, occurance, 0);
        dfs(u, in+1, line, occurance, 1);
    }
};

info trie;

info makeTrie(vvi candidates) {
    trie.clear();

    for(int i=0; i<candidates.size(); i++) {
        vi can = candidates[i];
        trie.add(can, i);
    }
    return trie;
}

bool asecheck(vi temp) {
    for(int i=0; i<temp.size(); i++) {
        vi cur;
        for(int j=0; j<temp.size(); j++) if(i != j) cur.pb(temp[j]);
        if(ase.find(cur) == ase.end()) return false;
    }
    return true;
}
vvi prune(vvi candidates) {
    vvi ans;
    for(int i=0; i<candidates.size(); i++) {
        if(asecheck(candidates[i])) ans.pb(candidates[i]);
    }
    return ans;
}


vvi join(vvi last) {
    vvi ans;
    for(int i=0; i<last.size(); i++) {
        sort(last[i].begin(), last[i].end());
    }

    for(int i=0; i<last.size(); i++) {
        for(int j=i+1; j<last.size(); j++) {
            assert(last[i].size() == last[j].size());
            bool ok = true;
            for(int k=0; k<last[i].size()-1; k++) {
                if(last[i][k] != last[j][k]) ok = false;
            }
            if(ok) {
                vi temp = last[i];
                temp.pb(last[j].back());
//                if(asecheck(temp))
                    ans.pb(temp);
            }
        }
    }
    return ans;
}


int main() {
    char fl[110] = "mushroom.txt";
    cout << "Please enter filename of the dataset: ";
    scanf("%s", fl);

    freopen(fl, "r", stdin);
//    freopen("out.txt", "w", stdout);

//    cout << "PAISE " << endl;

    string line;
    while(getline(cin, line)) {
        vi data;
        int item;
        stringstream ss;
        ss << line;
        while(ss >> item) {
            if(item != -1) {
                data.pb(item);
                items.insert(item);
            }
        }
        sort(all(data));
        database.pb(data);
    }
//    cout << "BUG" << endl;
//    printDatabase();

    numberOfItem = items.size();
    numberOfTrans = database.size();
    MIN_SUP_COUNT_THRES = ceil(numberOfTrans * MIN_SUP_THRES / 100.0);

//    scanf("%d", &MIN_SUP_);
//    MIN_SUP_COUNT_THRES = ceil(numberOfTrans * MIN_SUP_THRES / 100.0);

    cout << "Number of Trans: " << numberOfTrans << " Number Of Items: " << items.size() << " Minimum Support Count: " << MIN_SUP_COUNT_THRES << endl;

    int ans = 0;

    for(int level = 0; ;level++) {
        vvi candidates;
        vvi pruned;

        if(level == 0) {
            for(auto it : items) {
                vi temp;
                temp.pb(it);
                candidates.pb(temp);
                pruned = candidates;
            }
        }
        else {
            candidates = join(L[level-1]);
            pruned = prune(candidates);
        }

//        candidates = pruned;
//        pruned = candidates;

        trie = makeTrie(pruned);
        vi occurance = trie.datasetTest(database);

        assert(occurance.size() == pruned.size());

        vvi li;
        for(int i=0; i<pruned.size(); i++) {
            if(occurance[i] >= MIN_SUP_COUNT_THRES) li.pb(pruned[i]);
        }

        L.pb(li);
        ase.clear();
        if(li.size() == 0) break;

        for(auto it : li) ase.insert(it);
        cout << "Candidates: " << candidates.size() << " Pruned: " << pruned.size() << " " << "L" << level+1 << ": " << li.size() << endl;

        ans += li.size();

//        cout << li.size() << endl;

    }

    cout << "TOTAL: " << ans << endl;

//    cout << "SD " << MAXI << endl;
}
