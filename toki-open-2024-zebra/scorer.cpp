#include <bits/stdc++.h>

std::ifstream inp;
std::ifstream out;
std::ifstream con;

void ac(std::string reason="") {
  std::cout << "AC\n";
  if (reason != "") {
    std::cout << reason << '\n';
  }
  exit(0);
}

void wa(std::string reason="") {
  std::cout << "WA\n";
  if (reason != "") {
    std::cout << reason << '\n';
  }
  exit(0);
}

void ok(double points, std::string reason="") {
  std::cout << "OK\n";
  std::cout << points;
  if (reason != "") {
    std::cout << " " << reason << '\n';
  }
  exit(0);
}

void registerScorer(int argc, char* argv[]) {
  if (argc != 4) {
    std::cout << "Must be run with arguments [input-file] [output-file] [contestant-output]\n";
    exit(0);
  }

  inp = std::ifstream(argv[1]);
  out = std::ifstream(argv[2]);
  con = std::ifstream(argv[3]);
}

template<class T> inline void readStream(std::ifstream &ifs, T &t) { if (!(ifs >> t)) wa(); }

int readInt(std::ifstream &ifs) { int t; readStream(ifs, t); return t; }
std::string readString(std::ifstream &ifs) { std::string t; readStream(ifs, t); return t; }

void eof() {
  std::string dummy;
  if (con >> dummy) wa();
}

int main(int argc, char* argv[]) {
  registerScorer(argc, argv);

  int subtask = readInt(inp);
  int N = readInt(inp);
  int R = readInt(inp);

  std::string queryString = readString(con);
  for (char c : queryString) {
    if (c != 'Q') {
      wa();
    }
  }
  std::string secretOutput = readString(con);
  if (secretOutput != "toki-open-2024-zebra_secret-output") {
    wa();
  }

  int guessedRoot = readInt(con);
  if (guessedRoot != R) {
    wa();
  }

  int Q = readInt(con);
  if ((int) queryString.size() != Q) {
    wa();
  }

  eof();

  if (subtask != 3) {
    if (Q > 20) {
      wa();
    }
    ac();
  }

  if (Q > 500) {
    wa();
  }
  if (Q > 20) {
    ok(((500 - Q) * (500 - Q) + 50000) / 4000);
  }
  ac();
}
