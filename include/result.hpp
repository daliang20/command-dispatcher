#ifndef RESULT_HPP
#define RESULT_HPP

template <typename T> struct Result {
  int resultCode = 0;
  T value;
};

#define assert_parameter(variable, action, error)                              \
  do {                                                                         \
    auto result = action;                                                      \
    if (result.resultCode != 0) {                                              \
      printf(#variable " Error\n");                                            \
      return result.resultCode;                                                \
    }                                                                          \
    variable = result.value;                                                   \
  } while (0)

#endif // RESULT_HPP