#include <iostream>
#include <string>

#include <unicode/uidna.h>


int main(int argv, char* argc[]) {
  std::cout << "Test ICU\n";

  UErrorCode err = U_ZERO_ERROR;
  // https://url.spec.whatwg.org/#idna
  // UseSTD3ASCIIRules = false
  // Nontransitional_Processing
  // CheckBidi = true
  // CheckJoiners = true
  const UIDNA* pidna = uidna_openUTS46(
    UIDNA_CHECK_BIDI
    | UIDNA_CHECK_CONTEXTJ
    | UIDNA_NONTRANSITIONAL_TO_ASCII
    | UIDNA_NONTRANSITIONAL_TO_UNICODE, &err);
  if (U_FAILURE(err)) {
    std::cerr << "failed to open UTS46 data with error: " << err << '\n';
    return 1;
  }

  const std::string name("A.ąčęėįšųūž.Z.lt");
  char dest[1024];

  UIDNAInfo info = UIDNA_INFO_INITIALIZER;
  int output_length = uidna_nameToASCII_UTF8(pidna,
    name.data(), static_cast<int32_t>(name.length()),
    dest, sizeof(dest),
    &info, &err);
  if (U_FAILURE(err) && info.errors != 0) {
    std::cerr << "uidna_nameToASCII_UTF8 error: " << err << " (" << info.errors << ")\n";
    return 2;
  }

  dest[output_length] = 0;
  std::cout << "OUTPUP: " << dest << '\n';

  return 0;
}
