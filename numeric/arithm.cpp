#define __DEBUG(depth, x)                                                      \
  if (true) {                                                                  \
    for (int dpth = 0; dpth < depth; ++dpth) {                                 \
      std::cout << "  ";                                                       \
    }                                                                          \
    std::cout << "at: " << __LINE__ << ",\t" << #x << " = " << x.size << ": "  \
              << io.getDec(x, *this) << " " << isSigned(x) << std::endl;       \
  }





