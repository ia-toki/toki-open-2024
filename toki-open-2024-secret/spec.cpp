#include <bits/stdc++.h>
#include <tcframe/spec.hpp>
using namespace std;
using namespace tcframe;

const int MIN_N = 2;
const int MAX_N = 1e5;
const int MAX_N_SUB2 = 2000;

const int MIN_M = 3;
const int MAX_M = 1e9;

const int MIN_Q = 1;
const int MAX_Q = 1e5;
const int MAX_Q_SUB2 = 2000;

class ProblemSpec : public BaseProblemSpec {
  protected:
    int N, M, Q;
    vector<int> A;
    vector<vector<int>> queries;
    vector<long long> ans;
    int expectedSize;

    void InputFormat() {
      LINE(N, M, Q);
      LINE(A % SIZE(N));
      LINES(queries) % SIZE(Q);
    }

    void BeforeOutputFormat() {
      expectedSize = count_if(queries.begin(), queries.end(), isQueryOfType1);
    }

    void OutputFormat() {
      LINES(ans) % SIZE(expectedSize);
    }

    void GradingConfig() {
      TimeLimit(2);
      MemoryLimit(512);
    }

    void Constraints() {
      CONS(MIN_N <= N && N <= MAX_N);
      CONS(MIN_M <= M && M <= MAX_M);
      CONS(MIN_Q <= Q && Q <= MAX_Q);
      CONS(isCorrectQueryFormat(queries));
      CONS(count_if(queries.begin(), queries.end(), isQueryOfType1) > 0);
    }

    // The toggle event will never happen.
    void Subtask1() {
      CONS(all_of(queries.begin(), queries.end(), isQueryOfType1) == true);
      Points(1);
    }

    // N <= 2000, Q <= 2000
    void Subtask2() {
      CONS(N <= MAX_N_SUB2);
      CONS(Q <= MAX_Q_SUB2);
      Points(1);
    }

    // A[i] < A[i+1]
    void Subtask3() {
      CONS(is_sorted(A.begin(), A.end()));
      CONS(adjacent_find(A.begin(), A.end()) == A.end());
      Points(1);
    }

    // Road (M - 1) will always be under surveillance each time a query event happens.
    void Subtask4() {
      CONS(roadXAlwaysUnderSurveillance(M-1, queries));
      Points(1);
    }

    // S == 0, T == N-1
    void Subtask5() {
      CONS(all_of(queries.begin(), queries.end(), allOfQueryRangeEqual({0, N-1})));
      Points(1);
    }

    // full
    void Subtask6() {
      Points(1);
    }

    private:
    bool isCorrectQueryFormat(const vector<vector<int>>& queries) {
      for (vector<int> query : queries) {
        if (query[0] == 0) {
          if (query.size() != 2) return false;

          int R = query[1];
          if (!(0 <= R && R <= M-1)) return false;

        } else if (query[0] == 1) {
          if (query.size() != 3) return false;

          int S = query[1], T = query[2];
          if (!(0 <= S && S < T)) return false;
          if (!(S < T && T <= N-1)) return false;
        } else {
          return false;
        }
      }
      return true;
    }

    bool roadXAlwaysUnderSurveillance(int X, const vector<vector<int>>& queries) {
      bool isUnderSurveillance = false;
      for (vector<int> query : queries) {
        if (query[0] == 0) {
          int R = query[1];
          if (R == X) {
            isUnderSurveillance = !isUnderSurveillance;
          }
        }

        if (query[0] == 1) {
          if (!isUnderSurveillance) {
            return false;
          }
        }
      }
      return true;
    }

    function<bool(vector<int>)> isQueryOfType1 = [&](const vector<int> &query) { 
      return query[0] == 1; 
    };

    function<bool(vector<int>)> allOfQueryRangeEqual(pair<int, int> range) {
      return [&, range](const vector<int> &query) { 
        return query[0] == 0 || (query[1] == range.first && query[2] == range.second); 
      };
    }
};

class TestSpec : public BaseTestSpec<ProblemSpec> {
protected:
  void SampleTestCase1() {
    Subtasks({2, 6});
    Input({
      "4 5 8",
      "1 1 4 2",
      "0 4",
      "1 1 2",
      "0 1",
      "1 1 2",
      "1 0 1",
      "0 1",
      "0 4",
      "1 0 3"
    });
    Output({
      "3",
      "-1",
      "0",
      "4",
    });
  }

  void BeforeTestCase() {
    // A.clear();
  }

  void TestGroup1() {
    Subtasks({1, 2, 5, 6});

    CASE(N = MIN_N; M = MIN_M; Q = MAX_Q_SUB2; A = {M / 2, 0}; queries = rndQueriesNoToggle(N, M, Q));
  }

  void TestGroup2() {
    Subtasks({1, 2, 6});

    CASE(N = MIN_N+1; M = MAX_M; Q = MAX_Q_SUB2; A = rndDistinctArray(N); queries = rndQueriesNoToggle(N, M, Q));
  }

  void TestGroup3() {
    Subtasks({1, 6});

    CASE(N = MAX_N; M = MAX_M; Q = MAX_Q; A = rndArray(N); queries = rndQueriesNoToggle(N, M, Q));
    CASE(N = MAX_N-1; M = MAX_M; Q = MAX_Q; A = rndArray(N, M-1, M-1); queries = rndQueriesNoToggle(N, M, Q));
  }

  void TestGroup4() {
    Subtasks({2, 6});

    // random ranges
    CASE(N = MAX_N_SUB2; M = MAX_M; Q = MAX_Q_SUB2; A = rndArray(N); queries = rndQueries(N, M, Q, 1, OptWeightGen(1, 1, 1)));
    CASE(N = MAX_N_SUB2-1; M = MAX_M; Q = MAX_Q_SUB2; A = rndArray(N); queries = rndQueries(N, M, Q, 1, OptWeightGen(1, 1, 2)));
    CASE(N = MAX_N_SUB2-2; M = MAX_M; Q = MAX_Q_SUB2; A = rndArray(N); queries = rndQueries(N, M, Q, 1, OptWeightGen(2, 1, 3)));

    // small ranges
    CASE(N = MAX_N_SUB2; M = MAX_M; Q = MAX_Q_SUB2; A = rndArray(N); queries = rndQueries(N, M, Q, 2, OptWeightGen(2, 1, 3), rndRangeLenF(2, 2)));
    CASE(N = MAX_N_SUB2; M = MAX_M; Q = MAX_Q_SUB2; A = rndArray(N); queries = rndQueries(N, M, Q, 3, OptWeightGen(2, 1, 4), rndRangeLenF(2, 2)));
    CASE(N = MAX_N_SUB2; M = MAX_M; Q = MAX_Q_SUB2; A = rndArraySameAdj(N); queries = rndQueries(N, M, Q, 2, OptWeightGen(2, 1, 5), rndRangeLenF(2, 3)));

    // big ranges
    CASE(N = MAX_N_SUB2; M = MAX_M; Q = MAX_Q_SUB2; A = rndArray(N); queries = rndQueries(N, M, Q, 1, OptWeightGen(2, 1, 6), rndRangeLenF(N-100, N)));
    CASE(N = MAX_N_SUB2; M = MAX_M; Q = MAX_Q_SUB2; A = rndArraySameAdj(N); queries = rndQueries(N, M, Q, 2, OptWeightGen(2, 1, 6), rndRangeLenF(N/3, N/2)));

    // big ranges, high chance of same adj
    CASE(N = MAX_N_SUB2; M = MAX_M; Q = MAX_Q_SUB2; A = rndArraySameAdj(N, 0.6); queries = rndQueries(N, M, Q, 4, OptWeightGen(2, 1, 6), rndRangeLenF(N/3, N/2)));
    CASE(N = MAX_N_SUB2; M = MAX_M; Q = MAX_Q_SUB2; A = rndArraySameAdj(N, 0.9); queries = rndQueries(N, M, Q, 7, OptWeightGen(2, 1, 6), rndRangeLenF(N/3, N/2)));

    // alternating
    CASE(N = MAX_N_SUB2; M = MAX_M; Q = MAX_Q_SUB2; A = rndArray(N); queries = rndQueries(N, M, Q, 1, OptAlternatingGen(1), rndRangeLenF(N/3, N)));
    CASE(N = MAX_N_SUB2; M = MAX_M; Q = MAX_Q_SUB2; A = rndArray(N); queries = rndQueries(N, M, Q, 1, OptAlternatingGen(N), rndRangeLenF(N/3, N)));

    // one element
    CASE(N = MAX_N_SUB2; M = MAX_M; Q = MAX_Q_SUB2; A = rndArray(N, M-1, M-1); queries = rndQueries(N, M, Q, 1, OptAlternatingGen(1), rndRangeLenF(N/3, N)));

    // OptSelectRangeGen generator
    CASE(  
      N = MAX_N_SUB2; M = MAX_M; Q = MAX_Q_SUB2; 
      A = rndArraySameAdj(N); 
      OptSelectRangeGen queryGen = OptSelectRangeGen(N, M, A, *this);
      queries = queryGen.getQueries(Q);
    );
    CASE(  
      N = MAX_N_SUB2; M = MAX_M; Q = MAX_Q_SUB2; 
      A = rndArraySameAdj(N); 
      OptSelectRangeGen queryGen = OptSelectRangeGen(N, M, A, *this);
      queryGen.setSelectOtherRange(2, 20, 0.9);
      queryGen.setQueriesRnd({1, 1, 2, 2, 3});
      queryGen.addOnDemandToggle(M-1);
      queryGen.getQueries(Q/2);
      queryGen.addOnDemandToggle(0);
      queries = queryGen.getQueries(Q);
    );
    CASE(  
      N = MAX_N_SUB2; M = MAX_M; Q = MAX_Q_SUB2; 
      A = rndArraySameAdj(N); 
      OptSelectRangeGen queryGen = OptSelectRangeGen(N, M, A, *this);
      queryGen.setSelectOtherRange(2, 100, 0.9);
      queryGen.setQueriesRnd({1, 2, 2, 3});
      queries = queryGen.getQueries(Q);
    );

    // high chance of same adj elements
    CASE(  
      N = MAX_N_SUB2; M = MAX_M; Q = MAX_Q_SUB2; 
      A = rndArraySameAdj(N, 0.9); 
      OptSelectRangeGen queryGen = OptSelectRangeGen(N, M, A, *this);
      queryGen.setSelectOtherRange(2, 100, 0.9);
      queryGen.setQueriesRnd({1, 2, 2, 3});
      queries = queryGen.getQueries(Q);
    );
  }

  void TestGroup5() {
    Subtasks({3, 6});
    
    // small ranges
    CASE(N = MAX_N  ; M = MAX_M; Q = MAX_Q; A = rndSortedDistinctArray(N); queries = rndQueries(N, M, Q, 2, OptWeightGen(2, 1, 3), rndRangeLenF(2, 2)));
    CASE(N = MAX_N-1; M = MAX_M; Q = MAX_Q; A = rndSortedDistinctArray(N); queries = rndQueries(N, M, Q, 3, OptWeightGen(2, 1, 4), rndRangeLenF(2, 2)));
    CASE(N = MAX_N-2; M = MAX_M; Q = MAX_Q; A = rndSortedDistinctArray(N); queries = rndQueries(N, M, Q, 2, OptWeightGen(2, 1, 5), rndRangeLenF(2, 3)));
    CASE(N = MAX_N-2; M = MAX_M; Q = MAX_Q; A = rndSortedDistinctArray(N); queries = rndQueries(N, M, Q, 5, OptWeightGen(3, 2, 7), rndRangeLenF(2, 5)));
    CASE(N = MAX_N-2; M = MAX_M; Q = MAX_Q; A = rndSortedDistinctArray(N); queries = rndQueries(N, M, Q, 2, OptWeightGen(3, 2, 7), rndRangeLenF(2, 10)));

    // big ranges
    CASE(N = MAX_N; M = MAX_M; Q = MAX_Q; A = rndSortedDistinctArray(N); queries = rndQueries(N, M, Q, 1, OptWeightGen(2, 1, 6), rndRangeLenF(N-100, N)));
    CASE(N = MAX_N; M = MAX_M; Q = MAX_Q; A = rndSortedDistinctArray(N); queries = rndQueries(N, M, Q, 2, OptWeightGen(2, 1, 6), rndRangeLenF(N/3, N/2)));
    CASE(N = MAX_N; M = MAX_M; Q = MAX_Q; A = rndSortedDistinctArray(N); queries = rndQueries(N, M, Q, 2, OptWeightGen(3, 2, 7), rndRangeLenF(N/3, N/2)));
    CASE(N = MAX_N; M = MAX_M; Q = MAX_Q; A = rndSortedDistinctArray(N); queries = rndQueries(N, M, Q, 3, OptWeightGen(3, 2, 7), rndRangeLenF(N/3, N/2)));

    // OptSelectRangeGen generator
    CASE(  
      N = MAX_N; M = MAX_M; Q = MAX_Q; 
      A = rndSortedDistinctArray(N); 
      OptSelectRangeGen queryGen = OptSelectRangeGen(N, M, A, *this);
      queries = queryGen.getQueries(Q);
    );
    CASE(  
      N = MAX_N; M = MAX_M; Q = MAX_Q; 
      A = rndSortedDistinctArray(N); 
      OptSelectRangeGen queryGen = OptSelectRangeGen(N, M, A, *this);
      queryGen.setQueriesRnd({1, 1, 1, 2, 3});
      queries = queryGen.getQueries(Q);
    );
    CASE(  
      N = MAX_N; M = MAX_M; Q = MAX_Q; 
      A = rndSortedDistinctArray(N); 
      OptSelectRangeGen queryGen = OptSelectRangeGen(N, M, A, *this);
      queryGen.setSelectOtherRange(2, 20, 0.9);
      queryGen.setQueriesRnd({1, 1, 2, 2, 3});
      queryGen.addOnDemandToggle(M-1);
      queryGen.getQueries(Q/2);
      queryGen.addOnDemandToggle(0);
      queries = queryGen.getQueries(Q);
    );
    CASE(  
      N = MAX_N; M = MAX_M; Q = MAX_Q; 
      A = rndSortedDistinctArray(N); 
      OptSelectRangeGen queryGen = OptSelectRangeGen(N, M, A, *this);
      queryGen.setSelectOtherRange(2, 100, 0.9);
      queryGen.setQueriesRnd({1, 2, 2, 3});
      queries = queryGen.getQueries(Q);
    );

    CASE(N = 4; M = MAX_M; Q = MAX_Q; 
         A = {0, 1, M-2, M-1}; 
         queries = {
          {1, 0, 3},
          {0, M-2},
          {1, 0, 3},
          {1, 0, 2},
          {0, 1},
          {1, 0, 3},
          {1, 0, 2},
          {0, M-2},
          {1, 0, 3},
          {1, 0, 2},
          {0, 1},
         };
         repeatVectorElemUntilSize(queries, Q);
    );
  }


  void TestGroup6() {
    Subtasks({4, 6});
    
    // small ranges
    CASE(N = MAX_N  ; M = MAX_M  ; Q = MAX_Q; A = rndArray(N);        queries = rndQueries(N, M, Q, 2, OptWeightGen(2, 1, 3), rndRangeLenF(2, 2), poolBasedToggleRndSub4(M, A), true));
    CASE(N = MAX_N-1; M = MAX_M  ; Q = MAX_Q; A = rndArraySameAdj(N); queries = rndQueries(N, M, Q, 3, OptWeightGen(2, 1, 4), rndRangeLenF(2, 2), poolBasedToggleRndSub4(M, A), true));
    CASE(N = MAX_N-2; M = MAX_M-1; Q = MAX_Q; A = rndArraySameAdj(N); queries = rndQueries(N, M, Q, 2, OptWeightGen(2, 1, 5), rndRangeLenF(2, 3), poolBasedToggleRndSub4(M, A), true));

    // small ranges sorted A
    CASE(N = MAX_N-2; M = MAX_M  ; Q = MAX_Q; A = rndArray(N);        sort(A.begin(), A.end()); queries = rndQueries(N, M, Q, 10, OptWeightGen(3, 2, 7), rndRangeLenF(2,  5), poolBasedToggleRndSub4(M, A), true));
    CASE(N = MAX_N-2; M = MAX_M  ; Q = MAX_Q; A = rndArraySameAdj(N); sort(A.begin(), A.end()); queries = rndQueries(N, M, Q,  5, OptWeightGen(4, 3, 9), rndRangeLenF(2, 10), poolBasedToggleRndSub4(M, A), true));
    CASE(N = MAX_N-2; M = MAX_M-1; Q = MAX_Q; A = rndArraySameAdj(N); sort(A.begin(), A.end()); queries = rndQueries(N, M, Q, 15, OptWeightGen(4, 3, 9), rndRangeLenF(2,  7), poolBasedToggleRndSub4(M, A), true));

    // big ranges
    CASE(N = MAX_N; M = MAX_M; Q = MAX_Q; A = rndArray(N);        queries = rndQueries(N, M, Q, 1, OptWeightGen(2, 1, 6), rndRangeLenF(N-100, N), poolBasedToggleRndSub4(M, A), true));
    CASE(N = MAX_N; M = MAX_M; Q = MAX_Q; A = rndArraySameAdj(N); queries = rndQueries(N, M, Q, 2, OptWeightGen(2, 1, 6), rndRangeLenF(N/3, N/2), poolBasedToggleRndSub4(M, A), true));
    CASE(N = MAX_N; M = MAX_M; Q = MAX_Q; A = rndArraySameAdj(N); queries = rndQueries(N, M, Q, 5, OptWeightGen(3, 2, 8), rndRangeLenF(N/3, N/2), poolBasedToggleRndSub4(M, A), true));

    // OptSelectRangeGen generator
    CASE(  
      N = MAX_N; M = MAX_M; Q = MAX_Q; 
      A = rndArray(N); 
      OptSelectRangeGen queryGen = OptSelectRangeGen(N, M, A, *this);
      queryGen.setBeforeSelectRange([&]() { queryGen.addOnDemandToggle(M-1); });
      queries = queryGen.getQueries(Q);
    );
    CASE(  
      N = MAX_N; M = MAX_M; Q = MAX_Q; 
      A = rndArray(N); 
      OptSelectRangeGen queryGen = OptSelectRangeGen(N, M, A, *this);
      queryGen.setBeforeSelectRange([&]() { queryGen.addOnDemandToggle(M-1); });
      queryGen.setQueriesRnd({1, 1, 1, 2, 3});
      queries = queryGen.getQueries(Q);
    );
    CASE(  
      N = MAX_N; M = MAX_M; Q = MAX_Q; 
      A = rndArraySameAdj(N); 
      OptSelectRangeGen queryGen = OptSelectRangeGen(N, M, A, *this);
      queryGen.setBeforeSelectRange([&]() { queryGen.addOnDemandToggle(M-1); });
      queryGen.setSelectOtherRange(2, 20, 0.9);
      queryGen.setQueriesRnd({1, 1, 2, 2, 3});
      queries = queryGen.getQueries(Q);
    );
    CASE(  
      N = MAX_N; M = MAX_M; Q = MAX_Q; 
      A = rndArraySameAdj(N); 
      OptSelectRangeGen queryGen = OptSelectRangeGen(N, M, A, *this);
      queryGen.setBeforeSelectRange([&]() { queryGen.addOnDemandToggle(M-1); });
      queryGen.setSelectOtherRange(2, 100, 0.9);
      queryGen.setQueriesRnd({1, 2, 2, 3});
      queries = queryGen.getQueries(Q);
    );

    // high chance of same adj elements
    CASE(  
      N = MAX_N; M = MAX_M; Q = MAX_Q; 
      A = rndArraySameAdj(N, 0.9); 
      OptSelectRangeGen queryGen = OptSelectRangeGen(N, M, A, *this);
      queryGen.setBeforeSelectRange([&]() { queryGen.addOnDemandToggle(M-1); });
      queryGen.setSelectOtherRange(2, 100, 0.9);
      queryGen.setQueriesRnd({1, 1, 2, 3});
      queries = queryGen.getQueries(Q);
    );
  }
  
  void TestGroup7() {
    Subtasks({5, 6});
    
    // randoms OptWeightGen
    CASE(N = MAX_N  ; M = MAX_M  ; Q = MAX_Q; A = rndArray(N);             queries = rndQueries(N, M, Q, 2, OptWeightGen(2, 1, 3), rndRangeLenF(N, N)));
    CASE(N = MAX_N-2; M = MAX_M-1; Q = MAX_Q; A = rndArraySameAdj(N);      queries = rndQueries(N, M, Q, 2, OptWeightGen(2, 1, 5), rndRangeLenF(N, N)));
    CASE(N = MAX_N-1; M = MAX_M  ; Q = MAX_Q; A = rndArraySameAdj(N, 0.9); queries = rndQueries(N, M, Q, 3, OptWeightGen(2, 1, 4), rndRangeLenF(N, N)));

    // randoms OptAlternatingGen
    CASE(N = MAX_N  ; M = MAX_M  ; Q = MAX_Q; A = rndArray(N);             queries = rndQueries(N, M, Q, 1, OptAlternatingGen(1), rndRangeLenF(N, N)));
    CASE(N = MAX_N-1; M = MAX_M  ; Q = MAX_Q; A = rndArraySameAdj(N, 0.9); queries = rndQueries(N, M, Q, 1, OptAlternatingGen(1), rndRangeLenF(N, N)));

    // one element
    CASE(N = MAX_N  ; M = MAX_M  ; Q = MAX_Q; A = rndArray(N, M-1, M-1);   queries = rndQueries(N, M, Q, 1, OptAlternatingGen(1), rndRangeLenF(N, N)));

    // OptSelectRangeGen generator
    // because query = {0, N-1}, set query times to be 1
    CASE(  
      N = MAX_N; M = MAX_M; Q = MAX_Q; 
      A = rndArray(N); 
      OptSelectRangeGen queryGen = OptSelectRangeGen(N, M, A, *this);
      queryGen.setBeforeSelectRange(NULL);
      queryGen.setSelectOtherRange(2, 1, 0.5);
      queryGen.setQueryInSelectRange(1);
      queryGen.setQueriesRnd({2, 3});
      queries = queryGen.getQueries(Q);
      QueriesUtils().setAllQuery(queries, {0, N-1});
    );
    CASE(  
      N = MAX_N; M = MAX_M; Q = MAX_Q; 
      A = rndArray(N); 
      OptSelectRangeGen queryGen = OptSelectRangeGen(N, M, A, *this);
      queryGen.setSelectOtherRange(2, 1, 0.9);
      queryGen.setQueryInSelectRange(1);
      queryGen.setQueriesRnd({2, 2, 3});
      queries = queryGen.getQueries(Q);
      QueriesUtils().setAllQuery(queries, {0, N-1});
    );
    CASE(  
      N = MAX_N; M = MAX_M; Q = MAX_Q; 
      A = rndArraySameAdj(N); 
      OptSelectRangeGen queryGen = OptSelectRangeGen(N, M, A, *this);
      queryGen.setSelectOtherRange(2, 1, 0.5);
      queryGen.setQueryInSelectRange(1);
      queryGen.setQueriesRnd({2, 2, 3});
      queries = queryGen.getQueries(Q);
      QueriesUtils().setAllQuery(queries, {0, N-1});
    );
    CASE(  
      N = MAX_N; M = MAX_M; Q = MAX_Q; 
      A = rndArraySameAdj(N); 
      OptSelectRangeGen queryGen = OptSelectRangeGen(N, M, A, *this);
      queryGen.setSelectOtherRange(2, 1, 0.7);
      queryGen.setQueryInSelectRange(1);
      queryGen.setQueriesRnd({2, 3});
      queries = queryGen.getQueries(Q);
      QueriesUtils().setAllQuery(queries, {0, N-1});
    );

    // high chance of same adj elements
    CASE(  
      N = MAX_N; M = MAX_M; Q = MAX_Q; 
      A = rndArraySameAdj(N, 0.9); 
      OptSelectRangeGen queryGen = OptSelectRangeGen(N, M, A, *this);
      queryGen.setSelectOtherRange(2, 100, 0.9);
      queryGen.setQueriesRnd({2, 3});
      queries = queryGen.getQueries(Q);
      QueriesUtils().setAllQuery(queries, {0, N-1});
    );
    CASE(  
      N = MAX_N; M = MAX_M; Q = MAX_Q; 
      A = rndArraySameAdj(N, 0.9); 
      OptSelectRangeGen queryGen = OptSelectRangeGen(N, M, A, *this);
      queryGen.setSelectOtherRange(2, 100, 0.2);
      queryGen.setQueriesRnd({2, 2, 3});
      queries = queryGen.getQueries(Q);
      QueriesUtils().setAllQuery(queries, {0, N-1});
    );
  }

  void TestGroup8() {
    Subtasks({6});

    // random ranges
    CASE(N = MAX_N; M = MAX_M; Q = MAX_Q; A = rndArray(N); queries = rndQueries(N, M, Q, 1, OptWeightGen(1, 1, 1)));
    CASE(N = MAX_N-1; M = MAX_M; Q = MAX_Q; A = rndArray(N); queries = rndQueries(N, M, Q, 1, OptWeightGen(1, 1, 2)));
    CASE(N = MAX_N-2; M = MAX_M; Q = MAX_Q; A = rndArray(N); queries = rndQueries(N, M, Q, 1, OptWeightGen(2, 1, 3)));

    // small ranges
    CASE(N = MAX_N; M = MAX_M; Q = MAX_Q; A = rndArray(N); queries = rndQueries(N, M, Q, 2, OptWeightGen(2, 1, 3), rndRangeLenF(2, 2)));
    CASE(N = MAX_N; M = MAX_M; Q = MAX_Q; A = rndArray(N); queries = rndQueries(N, M, Q, 3, OptWeightGen(2, 1, 4), rndRangeLenF(2, 2)));
    CASE(N = MAX_N; M = MAX_M; Q = MAX_Q; A = rndArraySameAdj(N); queries = rndQueries(N, M, Q, 2, OptWeightGen(2, 1, 5), rndRangeLenF(2, 3)));

    // big ranges
    CASE(N = MAX_N; M = MAX_M; Q = MAX_Q; A = rndArray(N); queries = rndQueries(N, M, Q, 1, OptWeightGen(2, 1, 6), rndRangeLenF(N-100, N)));
    CASE(N = MAX_N; M = MAX_M; Q = MAX_Q; A = rndArraySameAdj(N); queries = rndQueries(N, M, Q, 2, OptWeightGen(2, 1, 6), rndRangeLenF(N/3, N/2)));

    // big ranges, high chance of same adj
    CASE(N = MAX_N; M = MAX_M; Q = MAX_Q; A = rndArraySameAdj(N, 0.6); queries = rndQueries(N, M, Q, 4, OptWeightGen(2, 1, 6), rndRangeLenF(N/3, N/2)));
    CASE(N = MAX_N; M = MAX_M; Q = MAX_Q; A = rndArraySameAdj(N, 0.9); queries = rndQueries(N, M, Q, 7, OptWeightGen(2, 1, 6), rndRangeLenF(N/3, N/2)));

    // alternating
    CASE(N = MAX_N; M = MAX_M; Q = MAX_Q; A = rndArray(N); queries = rndQueries(N, M, Q, 1, OptAlternatingGen(1), rndRangeLenF(N/3, N)));
    CASE(N = MAX_N; M = MAX_M; Q = MAX_Q; A = rndArray(N); queries = rndQueries(N, M, Q, 1, OptAlternatingGen(N), rndRangeLenF(N/3, N)));

    // one element
    CASE(N = MAX_N; M = MAX_M; Q = MAX_Q; A = rndArray(N, M-1, M-1); queries = rndQueries(N, M, Q, 1, OptAlternatingGen(1), rndRangeLenF(N/3, N)));

    // maximum number of toggle
    CASE(N = MAX_N; M = MAX_M; Q = MAX_Q; A = rndArray(N, M-1, M-1); queries = rndQueries(N, M, Q, 1, OptAlternatingGen(0), rndRangeLenF(N/3, N)); queries.back() = {1, 0, N-2});

    // OptSelectRangeGen generator
    CASE(  
      N = MAX_N; M = MAX_M; Q = MAX_Q; 
      A = rndArraySameAdj(N); 
      OptSelectRangeGen queryGen = OptSelectRangeGen(N, M, A, *this);
      queries = queryGen.getQueries(Q);
    );
    CASE(  
      N = MAX_N; M = MAX_M; Q = MAX_Q; 
      A = rndArraySameAdj(N); 
      OptSelectRangeGen queryGen = OptSelectRangeGen(N, M, A, *this);
      queryGen.setSelectOtherRange(2, 20, 0.9);
      queryGen.setQueriesRnd({1, 1, 2, 2, 3});
      queryGen.addOnDemandToggle(M-1);
      queryGen.getQueries(Q/2);
      queryGen.addOnDemandToggle(0);
      queries = queryGen.getQueries(Q);
    );
    CASE(  
      N = MAX_N; M = MAX_M; Q = MAX_Q; 
      A = rndArraySameAdj(N); 
      OptSelectRangeGen queryGen = OptSelectRangeGen(N, M, A, *this);
      queryGen.setSelectOtherRange(2, 100, 0.9);
      queryGen.setQueriesRnd({1, 2, 2, 3});
      queries = queryGen.getQueries(Q);
    );

    // high chance of same adj elements
    CASE(  
      N = MAX_N; M = MAX_M; Q = MAX_Q; 
      A = rndArraySameAdj(N, 0.9); 
      OptSelectRangeGen queryGen = OptSelectRangeGen(N, M, A, *this);
      queryGen.setSelectOtherRange(2, 100, 0.9);
      queryGen.setQueriesRnd({1, 2, 2, 3});
      queries = queryGen.getQueries(Q);
    );

  }

private:

  vector<int> rndArray(int N, int lo = 0, int hi = -1) {
    if (hi == -1) hi = M-1;
    vector<int> vec(N);
    for (int &elem : vec) elem = rnd.nextInt(lo, hi);
    return vec;
  }

  vector<int> rndArraySameAdj(int N, double adjSameChance = 0.01, int lo = 0, int hi = -1) {
    if (hi == -1) hi = M-1;
    vector<int> vec(N);
    int last = -1;
    for (int &elem : vec) {
      if (last != -1 && rnd.nextDouble(1) <= adjSameChance) {
        elem = last;
      } else {
        elem = rnd.nextInt(lo, hi);
      }
      last = elem;
    }
    return vec;
  }

  vector<int> rndDistinctArray(int N, int lo = 0, int hi = -1) {
    if (hi == -1) hi = M-1;
    if (hi - lo + 1 >= N * 20) {
      unordered_set<int> setRes;
      while (setRes.size() < N) {
        setRes.emplace(rnd.nextInt(lo, hi));
      }
      return vector<int>(setRes.begin(), setRes.end());

    } else {
      vector<int> res(hi - lo + 1);
      iota(res.begin(), res.end(), lo);
      rnd.shuffle(res.begin(), res.end());
      return res;
    }
  }

  vector<int> rndSortedDistinctArray(int N, int lo = 0, int hi = -1) {
    vector<int> vec = rndDistinctArray(N, lo, hi);
    sort(vec.begin(), vec.end());
    return vec;
  }

  template <class T>
  T rndPick(const vector<T> &vec) {
    return vec[rnd.nextLongLong(0, vec.size() - 1)];
  }

  template <class T>
  T rndPick(const initializer_list<T> &lst) {
    vector<int> vec(lst);
    return vec[rnd.nextLongLong(0, lst.size() - 1)];
  }

  template <class T>
  void rndSwapBack(vector<T> &vec) {
    auto pickedIdx = rnd.nextLongLong(0, vec.size() - 1);
    swap(vec.back(), vec[pickedIdx]);
  }

  template <class T>
  T rndPop(vector<T> &vec) {
    if (vec.empty()) {
      throw runtime_error("vec is empty");
    }
    rndSwapBack(vec);
    T res = vec.back();
    vec.pop_back();
    return res;
  }

  template <class T>
  void rndPush(vector<T> &vec, T elem) {
    vec.emplace_back(elem);
    rndSwapBack(vec);
  }

  template <class T>
  void repeatVectorElemUntilSize(vector<T> &vec, int n) {
    int initSz = vec.size();
    while (vec.size() < n) {
      int curIdx = vec.size() % initSz;
      vec.emplace_back(vec[curIdx]);
    }
  }

  enum Operation { TOGGLE_ON, TOGGLE_OFF, QUERY };

  function<pair<int, int>(int, int)> rndRange = [&](int lo, int hi) {
    vector<int> range = rndDistinctArray(2, lo, hi);
    return make_pair(min(range[0], range[1]), max(range[0], range[1]));
  };

  function<pair<int, int>(int, int)> rndRangeLenF(const int minLen, const int maxLen) {
    return [&, minLen, maxLen](int lo, int hi) {
      int len = rnd.nextInt(minLen, maxLen);
      int l = rnd.nextInt(lo, hi-len+1);
      return make_pair(l, l+len-1);
    };
  }

  function<int(Operation)> poolBasedToggleRnd(int poolSize, int lo, int hi) {
    vector<int> onPool = {};
    vector<int> offPool = rndDistinctArray(poolSize, lo, hi);
    return [&, onPool, offPool](Operation opt) mutable {
      int R;
      if (opt == TOGGLE_ON) {
        R = rndPop(offPool);
        rndPush(onPool, R);
        // queries.push_back({0, R});
      }

      if (opt == TOGGLE_OFF) {
        R = rndPop(onPool);
        rndPush(offPool, R);
        // queries.push_back({0, R});
      }
      return R;
    };
  }

  function<int(Operation)> poolBasedToggleRndSub4(
    int M,
    const vector<int> &A, 
    int otherRangePoolSize = -1, 
    int lastRangePoolSize = -1,
    double chanceToToggleOnM = 0.95,
    double chanceToToggleOffM = 0.05
  ) {
    vector<int> sortedA = A;
    sort(sortedA.begin(), sortedA.end());
    if (otherRangePoolSize == -1) {
      otherRangePoolSize = sortedA.size() * 5;
    }
    if (lastRangePoolSize == -1) {
      lastRangePoolSize = min((int) sortedA.size() * 2, (sortedA[0]+M - sortedA.back()) / 2);
    }

    vector<int> otherRangeOnPool = {};
    vector<int> otherRangeOffPool = rndDistinctArray(otherRangePoolSize, sortedA[0], sortedA.back()-1);

    vector<int> lastRangeOnPool = {};
    vector<int> lastRangeOffPool = rndDistinctArray(lastRangePoolSize, sortedA.back(), sortedA[0]+M-1);

    auto it = find(lastRangeOffPool.begin(), lastRangeOffPool.end(), M-1);
    if (it != lastRangeOffPool.end()) {
      lastRangeOffPool.erase(it);
    }

    bool isRoadMBlocked = false;
    return [&, otherRangeOnPool, otherRangeOffPool, lastRangeOnPool, lastRangeOffPool, chanceToToggleOffM, chanceToToggleOnM, M, isRoadMBlocked](Operation opt) mutable {
      int R;
      // cout << "get R \n";
      if (opt == TOGGLE_ON) {
        if (!isRoadMBlocked && rnd.nextDouble(1) <= chanceToToggleOnM) {
          R = M-1;
          isRoadMBlocked = true;
        } else {
          if (rnd.nextInt(2) == 1 || lastRangeOffPool.empty()) {
            // use other range pool
            R = rndPop(otherRangeOffPool);
            rndPush(otherRangeOnPool, R);
          } else {
            R = rndPop(lastRangeOffPool);
            rndPush(lastRangeOnPool, R);
          }
        }
      }

      if (opt == TOGGLE_OFF) {
        if ((isRoadMBlocked && rnd.nextDouble(1) <= chanceToToggleOffM) || (otherRangeOnPool.empty() && lastRangeOnPool.empty())) {
          R = M-1;
          isRoadMBlocked = false;
        } else {
          if ((!otherRangeOnPool.empty() && rnd.nextInt(2) == 1) || lastRangeOnPool.empty()) {
            // use other range pool
            R = rndPop(otherRangeOnPool);
            rndPush(otherRangeOffPool, R);
          } else {
            R = rndPop(lastRangeOnPool);
            rndPush(lastRangeOffPool, R);
          }
        }
      }
      return R % M;
    };
  }

  // weighted random query type
  struct OptWeightGen {
    int toggleOnChance, toggleOffChance, queryChance;

    OptWeightGen() : toggleOnChance(1), toggleOffChance(1), queryChance(4) {};
    OptWeightGen(int _toggleOnChance, int _toggleOffChance, int _queryChance) : 
      toggleOnChance(_toggleOnChance),
      toggleOffChance(_toggleOffChance),
      queryChance(_queryChance) {};

    Operation getQuery(TestSpec& testspec, bool canToggleOn = true, bool canToggleOff = true) {
      int _toggleOnChance = canToggleOn ? toggleOnChance : 0;
      int _toggleOffChance = canToggleOff ? toggleOffChance : 0;

      int rndNum = testspec.rnd.nextInt(_toggleOnChance + _toggleOffChance + queryChance);
      if (rndNum < _toggleOnChance) {
        return TOGGLE_ON;
      }
      if (rndNum < _toggleOnChance + _toggleOffChance) {
        return TOGGLE_OFF;
      }
      return QUERY;
    }
  };

  // alternating type query
  struct OptAlternatingGen {
    int queryCntPerCycle;
    int curIter;
    vector<Operation> optCycle; 

    OptAlternatingGen(int _queryCntPerCycle = 1) : queryCntPerCycle(_queryCntPerCycle) {
      optCycle = vector<Operation>(queryCntPerCycle+2, QUERY);
      optCycle[0] = TOGGLE_ON;
      optCycle.back() = TOGGLE_OFF;
      curIter = 0;
    };

    Operation getQuery(TestSpec& testspec, bool canToggleOn = true, bool canToggleOff = true) {
      while (true) {
        int realIter = curIter % optCycle.size();
        curIter++;
        Operation curOpt = optCycle[realIter];
        if (!canToggleOn && curOpt == TOGGLE_ON) continue;
        if (!canToggleOff && curOpt == TOGGLE_OFF) continue;
        return curOpt;
      }
    }
  };
  
  // first, select a range to be toggled on, and adjust the rest of the operation by the range
  // generator will do one thing of these possible action, 
  // 0. toggle range
  // 1. query randomly
  // 2. select other range to get query false (can select in the border)
  // 3. reset selected
  // 4. toggle in selected range (maybe)
  struct OptSelectRangeGen {
    int N, M;
    map<int, int> idxOfElem;
    vector<pair<int, int>> possibleRange;
    int idxOfSelectedRange;
    pair<int, int> selectedRange;
    TestSpec& testspec;

    bool haveSelectedRange;

    vector<vector<int>> queriesResult;
    map<int, set<int>> rangeToggledSet;
    // queue<pair<Operation, vector<int>>> operationQueues;

    OptSelectRangeGen(int N, int M, const vector<int> &A, TestSpec& _testspec) : N(N), M(M), testspec(_testspec) {
      vector<int> sortedA = A;
      sort(sortedA.begin(), sortedA.end());
      for (int i = 0; i < A.size(); i++) {
        idxOfElem[sortedA[i]] = i;
        int nxtElem = i+1 == sortedA.size() ? sortedA[0] + M : sortedA[i+1];
        if (sortedA[i] == nxtElem) continue;
        possibleRange.emplace_back(sortedA[i], nxtElem);
      }
    }

    const int MAX_TRIES = 20;

    int getRangeOfAnElem(int elem) {
      if (elem < possibleRange[0].first) {
        return possibleRange.size() - 1;
      } else {
        int idx = upper_bound(possibleRange.begin(), possibleRange.end(), make_pair(elem, MAX_M)) - possibleRange.begin() - 1;
        assert(0 <= idx && idx <= possibleRange.size() - 1);
        return idx; 
      }
    }

    bool tryToToggle(int elem, int rangeIdx) {
      assert(rangeIdx == getRangeOfAnElem(elem));
      if (!rangeToggledSet[rangeIdx].count(elem)) {
        rangeToggledSet[rangeIdx].insert(elem);
        queriesResult.push_back({0, elem % M});
        return true;
      }
      return false;
    }


    void addOnDemandToggle(int elem, int extraToggleCnt = 0) {
      assert(0 <= elem && elem < M);
      int idxRangeOfElem = getRangeOfAnElem(elem);
      if (!haveSelectedRange) {
        idxOfSelectedRange = idxRangeOfElem;
        selectedRange = possibleRange[idxOfSelectedRange];
        tryToToggle(elem, idxOfSelectedRange);
      } else {
        tryToToggle(elem, idxRangeOfElem);
      }
      if (extraToggleCnt > 0) {
        toggleXTimes(extraToggleCnt, idxRangeOfElem);
      }
    }

    void toggleXTimes(int X, int rangeIdx) {
      auto [l, r] = possibleRange[rangeIdx];
      r--;
      int toggledCnt = 0;
      int tries = 0;
      while (toggledCnt < X && tries < MAX_TRIES) {
        int selectedElem = testspec.rnd.nextInt(l, r);
        if (tryToToggle(selectedElem, rangeIdx)) {
          toggledCnt++;
        } else {
          tries++;
        }
      }
      haveSelectedRange = true;
    }

    struct SelectRangeProps {
      int toggleTimesModifier = 10;
      function<void()> beforeSelectRange = NULL;
    } selectRangeProps;

    void setSelectRange(int toggleTimesModifier) {
      selectRangeProps.toggleTimesModifier = toggleTimesModifier;
    }

    void setBeforeSelectRange(function<void()> func) {
      selectRangeProps.beforeSelectRange = func;
    }

    void selectRange() {
      if (selectRangeProps.beforeSelectRange != NULL) {
        selectRangeProps.beforeSelectRange();
      }
      if (!haveSelectedRange) {
        idxOfSelectedRange = testspec.rnd.nextInt(0, possibleRange.size()-1);
        selectedRange = possibleRange[idxOfSelectedRange];
      }
      int times = testspec.rnd.nextInt(1, selectRangeProps.toggleTimesModifier);
      toggleXTimes(times, idxOfSelectedRange);   
      haveSelectedRange = true;
    }

    void resetRange() {
      for (auto [_, toggledSet] : rangeToggledSet) {
        for (auto elem : toggledSet) {
          queriesResult.push_back({0, elem % M});
        } 
      }
      rangeToggledSet.clear();
      haveSelectedRange = false;
    }

    struct QueryInSelectRangeProps {
      int queryTimesModifier = 10;
    } queryInSelectRangeProps;

    void setQueryInSelectRange(int queryTimesModifier) {
      queryInSelectRangeProps.queryTimesModifier = queryTimesModifier;
    }

    void queryInSelectRange(int times = -1) {
      int queryCnt;
      if (times == -1) {
        queryCnt = testspec.rnd.nextInt(1, queryInSelectRangeProps.queryTimesModifier);
      } else {
        queryCnt = times;
      }
      auto [Al, Ar] = possibleRange[idxOfSelectedRange];
      int maxS = min(idxOfElem[Al], idxOfElem[Ar]);
      int minT = max(idxOfElem[Al], idxOfElem[Ar]);

      while (queryCnt > 0) {
        int S = testspec.rnd.nextInt(0, maxS);
        int T = testspec.rnd.nextInt(minT, N-1);
        queriesResult.push_back({1, S, T});
        queryCnt--;
      }
    }

    struct SelectOtherRangeProps {
      int toggleTimesModifier = 5;
      int queryTimesModifier = 10;
      double getAdjacentChance = 0.5;
    } selectOtherRangeProps;

    void setSelectOtherRange(int toggleTimesModifier, int queryTimesModifier, int getAdjacentChance) {
      selectOtherRangeProps.toggleTimesModifier = toggleTimesModifier;
      selectOtherRangeProps.queryTimesModifier = queryTimesModifier;
      selectOtherRangeProps.getAdjacentChance = getAdjacentChance;
    }

    void selectOtherRange() {
      // prioritize adjacent
      int toggleTimesModifier = selectOtherRangeProps.toggleTimesModifier;
      int queryTimesModifier = selectOtherRangeProps.queryTimesModifier;
      double getAdjacentChance = selectOtherRangeProps.getAdjacentChance;

      int newRangeIdx = -1;
      if (testspec.rnd.nextDouble(1) >= getAdjacentChance) {
        // get adjacent
        int tmpRangeIdx = testspec.rndPick({idxOfSelectedRange-1, idxOfSelectedRange+1});
        tmpRangeIdx = (tmpRangeIdx + possibleRange.size()) % possibleRange.size();
        if (!rangeToggledSet.count(tmpRangeIdx)) {
          newRangeIdx = tmpRangeIdx;
        }
      }
      
      if (newRangeIdx == -1) {
        for (int i = 1; i <= 10; i++) {
          int tmpRangeIdx = testspec.rnd.nextInt(0, possibleRange.size()-1);
          if (!rangeToggledSet.count(tmpRangeIdx)) {
            newRangeIdx = tmpRangeIdx;
            break;
          }
        }
      }

      if (newRangeIdx != -1) {
        assert(0 <= newRangeIdx && newRangeIdx < possibleRange.size());
        int toggleTimes = testspec.rnd.nextInt(1, toggleTimesModifier);
        toggleXTimes(toggleTimes, newRangeIdx);
        int queryTimes = testspec.rnd.nextInt(1, queryTimesModifier);
        queryInSelectRange(queryTimes);
      }
    }

    struct QueriesRndProps {
      vector<int> actionList = {1, 2, 3}; // can include duplicates to increase chances
    } queriesRndProps;

    void setQueriesRnd(vector<int> actionList) {
      queriesRndProps.actionList = actionList;
    }

    vector<vector<int>> getQueries(int Q) {
      while (queriesResult.size() <= Q) {
        if (!haveSelectedRange) {
          selectRange();
          queryInSelectRange();
        } else {
          int rndNum = testspec.rndPick(queriesRndProps.actionList);

          if (rndNum == 1) {
            queryInSelectRange();
          }
          if (rndNum == 2) {
            selectOtherRange();
          }
          if (rndNum == 3) {
            resetRange();
          }
        }
      }
      queriesResult.resize(Q);
      return queriesResult;
    }
  };

  template<class OptGenType>
  vector<vector<int>> rndQueries(
    int N, int M, int Q, 
    int maxActiveToggle, 
    OptGenType optGen, 
    function<pair<int, int>(int, int)> queryRangeRnd = NULL
  ) {
    return rndQueries(N, M, Q, maxActiveToggle, optGen, queryRangeRnd, NULL);
  }

  template<class OptGenType>
  vector<vector<int>> rndQueries(
    int N, int M, int Q, 
    int maxActiveToggle, 
    OptGenType optGen, // NOTE: check if this need pass by ref 
    function<pair<int, int>(int, int)> queryRangeRnd,
    function<int(Operation)> toggleRnd,
    bool onlyQueryIfMIsToggled = false
  ) {
    if (queryRangeRnd == NULL) {
      queryRangeRnd = rndRange;
    }
    if (toggleRnd == NULL) {
      toggleRnd = poolBasedToggleRnd(Q*5, 0, M-1);
    }
    int toggledCnt = 0;
    vector<vector<int>> queries;
    bool isMToggled = false;

    while (queries.size() < Q) {
      bool canToggleOn = toggledCnt < maxActiveToggle;
      bool canToggleOff = toggledCnt > 0;
      Operation op = optGen.getQuery(*this, canToggleOn, canToggleOff);

      if (op == TOGGLE_ON) {
        int R = toggleRnd(op);
        if (R == M-1) isMToggled = !isMToggled;
        toggledCnt++;
        queries.push_back({0, R});
      }

      if (op == TOGGLE_OFF) {
        int R = toggleRnd(op);
        if (R == M-1) isMToggled = !isMToggled;
        toggledCnt--;
        queries.push_back({0, R});
      }

      if (op == QUERY && (!onlyQueryIfMIsToggled || isMToggled)) {
        auto [S, T] = queryRangeRnd(0, N-1);
        queries.push_back({1, S, T});
      }
    }
    return queries;
  }

  vector<vector<int>> rndQueriesNoToggle(int N, int M, int Q) {
    return rndQueries(N, M, Q, 0, OptWeightGen(0, 0, 1));
  }


  class QueriesUtils {
    public:
    void setAllQuery(vector<vector<int>> &queries, pair<int, int> queryRange) {
      for (auto &query : queries) {
        if (query[0] == 1) {
          query = {1, queryRange.first, queryRange.second};
        }
      }
    }

    // NOTE: might add coordinate compressor for queries and A here.
  };
  // vector<vector<int>> rndQueriesSub4(int N, int M, int Q, int maxActiveToggle)
};