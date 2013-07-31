#include "parse_git_st.h"

int main(int argc, char** argv) {
    GitStatusParser git_status_parser;
    git_status_parser.parse();
    return 0;
}
