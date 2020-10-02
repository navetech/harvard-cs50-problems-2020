//#define _GNU_SOURCE

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);


int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}


// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
//    return false;

    // Repeat for each candidate
    for (int i = 0; i < candidate_count; i++)
    {
        // If name voted is a candidate,
        // Indicate that the voter has the candidate as their rank preference
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }

    // All candidates' names checked: name voted is not a candidate' name, return false
    return false;
}


// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
//    return;

    // Repeat for each pair of candidates in voter's ranks
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            // Increment number of voters who prefer candidate i over candidate j
            int winner = ranks[i];
            int loser = ranks[j];
            preferences[winner][loser]++;
        }
    }
}


// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
//    return;

    // Repeat for each pair of candidates
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            // If one candidate is preferred over other in the pair
            // Update the pairs list
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
}


struct _compareconfig
{
    bool invert;
    int *preferences;
    size_t size;
};

void sort_selection_r(void *base, size_t nmemb, size_t size,
                      int (*compar)(const void *, const void *, void *),
                      void *arg
                     );

void sort_bubble_r(void *base, size_t nmemb, size_t size,
                   int (*compar)(const void *, const void *, void *),
                   void *arg
                  );

void sort_insertion_r(void *base, size_t nmemb, size_t size,
                      int (*compar)(const void *, const void *, void *),
                      void *arg
                     );

void sort_merge_r(void *base, size_t nmemb, size_t size,
                  int (*compar)(const void *, const void *, void *),
                  void *arg
                 );

int compare_pairs(const void *pair1, const void *pair2, void *arg);

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
//    return;

    struct _compareconfig cmpcfg = {true, &preferences[0][0], MAX};

//    qsort_r(pairs, pair_count, sizeof(pair), compare_pairs, &cmpcfg);
//    sort_selection_r(pairs, pair_count, sizeof(pair), &compare_pairs, &cmpcfg);
//    sort_bubble_r(pairs, pair_count, sizeof(pair), &compare_pairs, &cmpcfg);
//    sort_insertion_r(pairs, pair_count, sizeof(pair), &compare_pairs, &cmpcfg);
    sort_merge_r(pairs, pair_count, sizeof(pair), &compare_pairs, &cmpcfg);
}


bool is_cycle(const int node_first, const int node);

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
//    return;

    // Repeat for each pair of candodates
    for (int i = 0; i < pair_count; i++)
    {
        // If the pair do not create a cycle in the graph, add the corresponding edge to the graph
        if (!is_cycle(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
}


// Print the winner of the election
void print_winner(void)
{
    // TODO
//    return;

    // Repeat for each node in the graph
    for (int i = 0; i < candidate_count; i++)
    {
        bool origin = false;
        bool destination = false;
        for (int j = 0; j < candidate_count; j++)
        {
            // Check if the node is a destination of an edge
            if (locked[j][i])
            {
                destination = true;
                break;
            }

            // Check if the node is a origin of an edge
            if (locked[i][j])
            {
                origin = true;
            }
        }

        // If the node is a origin of any edge and is not a destination of all edges,
        // this is the winner: print its name
        if (origin && !destination)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
}


void memswap(void *a, void *b, void *tmp, size_t size);


// Selection sort
void sort_selection_r(void *base, size_t nmemb, size_t size,
                      int (*compar)(const void *, const void *, void *),
                      void *arg
                     )
{
    void *tmp = malloc(size);
    if (tmp == NULL)
    {
        return;
    }

    for (int i = 0; i < nmemb - 1; i++)
    {
        void *pi = base + i * size;
        int *selected = pi;
        for (int j = i + 1; j < nmemb; j++)
        {
            void *pj = base + j * size;
            if (compar(pj, selected, arg) < 0)
            {
                selected = pj;
            }
        }

        if (selected != pi)
        {
            memswap(pi, selected, tmp, size);
        }
    }

    free(tmp);
}


// Bubble sort
void sort_bubble_r(void *base, size_t nmemb, size_t size,
                   int (*compar)(const void *, const void *, void *),
                   void *arg
                  )
{
    void *tmp = malloc(size);
    if (tmp == NULL)
    {
        return;
    }

    for (int i = 0; i < nmemb - 1; i++)
    {
        unsigned char swap = 0;
        for (int j = 0; j < nmemb - i - 1; j++)
        {
            void *pj = base + j * size;
            void *pjplus1 = pj + size;
            if (compar(pj, pjplus1, arg) > 0)
            {
                memswap(pj, pjplus1, tmp, size);
                swap = 1;
            }
        }

        if (swap == 0)
        {
            break;
        }
    }

    free(tmp);
}

// Insertion sort
void sort_insertion_r(void *base, size_t nmemb, size_t size,
                      int (*compar)(const void *, const void *, void *),
                      void *arg
                     )
{
    void *tmp = malloc(size);
    if (tmp == NULL)
    {
        return;
    }

    for (int i = 0; i < nmemb - 1; i++)
    {
        void *pi = base + i * size;
        void *piplus1 = pi + size;
        if (compar(piplus1, pi, arg) < 0)
        {
            memcpy(tmp, piplus1, size);

            void *pj = pi;
            do
            {
                void *pjplus1 = pj + size;;
                memcpy(pjplus1, pj, size);
                pj -= size;
            }
            while ((pj >= base) && (compar(tmp, pj, arg) < 0));

            void *pjplus1 = pj + size;;
            memcpy(pjplus1, tmp, size);
        }
    }

    free(tmp);
}


void merge_arrays(void *dest, const void *src1, size_t nmemb1, const void *src2, size_t nmemb2, size_t size,
                  int (*compar)(const void *, const void *, void *),
                  void *arg);

void sort_merge_r(void *base, size_t nmemb, size_t size,
                  int (*compar)(const void *, const void *, void *),
                  void *arg
                 )
{
    // if array's number of elements less or equal 1, array is sorted
    if (nmemb <= 1)
    {
        return;
    }

    void *merged = malloc(nmemb * size);
    if (merged == NULL)
    {
        return;
    }

    // Sort the left half of the array
    size_t nmemb_left = nmemb / 2;
    sort_merge_r(base, nmemb_left, size, compar, arg);

    // Sort the right half of the array
    size_t nmemb_right = nmemb - nmemb_left;
    void *base_right = base + nmemb_left * size;
    sort_merge_r(base_right, nmemb_right, size, compar, arg);

    // Merge the two sorted halves of the array
    merge_arrays(merged, base, nmemb_left, base_right, nmemb_right, size, compar, arg);
    memcpy(base, merged, nmemb * size);

    free(merged);
}


// Merge sorted arrays
void merge_arrays(void *dest, const void *src1, size_t nmemb1, const void *src2, size_t nmemb2, size_t size,
                  int (*compar)(const void *, const void *, void *),
                  void *arg)
{
    // Iterate the two source arrays
    const void *p1 = src1;
    const void *p2 = src2;
    void *pd = dest;
    while ((p1 < src1 + nmemb1 * size) && (p2 < src2 + nmemb2 * size))
    {
        // If source1 element is less than or equal to source2 element
        if (compar(p1, p2, arg) <= 0)
        {
            // Copy source 1 element to destinatio
            memcpy(pd, p1, size);
            p1 += size;
        }
        // If source1 element is greater than source2 element
        else
        {
            // Copy source 2 element to destinatio
            memcpy(pd, p2, size);
            p2 += size;
        }

        pd += size;
    }

    // Copy to destination remaining elements of source 1
    while (p1 < src1 + nmemb1 * size)
    {
        memcpy(pd, p1, size);
        p1 += size;
        pd += size;
    }

    // Copy to destination remaining elements of source 2
    while (p2 < src2 + nmemb2 * size)
    {
        memcpy(pd, p2, size);
        p2 += size;
        pd += size;
    }
}


// swap memory areas
void memswap(void *a, void *b, void *tmp, size_t size)
{
    memcpy(tmp, a, size);
    memcpy(a, b, size);
    memcpy(b, tmp, size);
}


// Compare strength of victory between two pairs
int compare_pairs(const void *pair1, const void *pair2, void *arg)
{
    // Return 0, if strengths are equal;
    // if invert is true: positive, if strength of pair 1 less then the one of pair 2; negative, otherwise
    // if invert is false negative, if strength of pair 1 less then the one of pair 2; positive, otherwise
    struct _compareconfig *cmpcfg = arg;

    int winner1 = ((pair *)pair1)->winner;
    int winner2 = ((pair *)pair2)->winner;
    int loser1 = ((pair *)pair1)->loser;
    int loser2 = ((pair *)pair2)->loser;
    bool invert = cmpcfg->invert;
    int *pref = cmpcfg->preferences;
    int size = cmpcfg->size;

    int strength1 = pref[winner1 * size + loser1];
    int strength2 = pref[winner2 * size + loser2];

    return invert ? strength2 - strength1 : strength1 - strength2;
}


bool is_cycle(const int node_first, const int node)
{
    // If current node is the first node, return true (create a cycle)
    if (node == node_first)
    {
        return true;
    }

    // Repeat for each node locked to the current node
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[node][i])
        {
            // if this nexto node create a cycle, return true (create a cycle)
            if (is_cycle(node_first, i))
            {
                return true;
            }

        }
    }

    // No node locked to the current node does create a cycle, return false (do not create a cycle)
    return false;

}