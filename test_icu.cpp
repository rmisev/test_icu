#include <iostream>
#include <string>

#include <unicode/uidna.h>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"


std::string domainToASCII(std::string name) {
  static UIDNA* pidna = nullptr;

  UErrorCode err = U_ZERO_ERROR;

  if (pidna == nullptr) {
    // https://url.spec.whatwg.org/#idna
    // UseSTD3ASCIIRules = false
    // Nontransitional_Processing
    // CheckBidi = true
    // CheckJoiners = true
    pidna = uidna_openUTS46(
      UIDNA_CHECK_BIDI
      | UIDNA_CHECK_CONTEXTJ
      | UIDNA_NONTRANSITIONAL_TO_ASCII
      | UIDNA_NONTRANSITIONAL_TO_UNICODE, &err);
    if (U_FAILURE(err)) {
      std::cerr << "failed to open UTS46 data with error: " << err << '\n';
      return "";
    }
  }

  char dest[1024];

  UIDNAInfo info = UIDNA_INFO_INITIALIZER;
  int output_length = uidna_nameToASCII_UTF8(pidna,
    name.data(), static_cast<int32_t>(name.length()),
    dest, sizeof(dest),
    &info, &err);
  if (U_FAILURE(err) || info.errors != 0) {
    std::cerr << "uidna_nameToASCII_UTF8 error: " << err << " (" << info.errors << ")\n";
    return "";
  }

  return std::string(dest, output_length);
}

TEST_CASE("testing the domainToASCII function") {
  CHECK(domainToASCII("A.ąčęėįšųūž.Z.lt") == "a.xn--2daq4ah5ionmc1c6e.z.lt");
  CHECK(domainToASCII("xn--abc") == "");
}
