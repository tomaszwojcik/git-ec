#ifndef __EXCEPTIONS_H
#define __EXCEPTIONS_H

#include <exception>

class NotAGitRepositoryException : public std::exception { 
};

class NothingToCommitException : public std::exception {
};

#endif
