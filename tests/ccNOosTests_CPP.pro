


############################################################ ARDP
# Absolutely Relative Directory Paths
# - relative to .PRO file's parent directory
# - from c:\IMS\CR\ccNOos\tests
# - or from /usr/REPOs/IMS/CR/ccNOos/tests
ccNOosDIR = $$absolute_path($${PWD}/../)
# - from c:\IMS\CR\ccOS\tests
# - or from /usr/REPOs/IMS/CR/ccOS/tests
#ccNOosDIR = $$absolute_path($${PWD}../ccNOos/)
# - from c:\IMS\CR\ccACU\Tests
# - or from /usr/REPOs/IMS/CR/ccACU\Tests
#ccNOosDIR = $$absolute_path($${PWD}../ccOS/ccNOos/)
# - finally, include the ccNOos.pri file

CONFIG += c++11

include(ccNOosTests.pri)

message("Includes:")
for(msg, INCLUDEPATH) {
    message($$msg)
}
message("Headers:")
for(msg, HEADERS) {
    message($$msg)
}
message("Sources:")
for(msg, SOURCES) {
    message($$msg)
}

