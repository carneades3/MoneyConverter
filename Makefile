CXX = g++
CPPFLAGS = -g -O0 -Wfatal-errors -Wall -Wextra  -std=c++14 -I/home/mat/asio-1.18.2/include  -I/home/mat/json/include #-Wconversion
CXX = clang++
#  -fcompare-debug-second -Wconversion
GCC_CPPFLAGS = -DMESSAGE='"Compiled with GCC"'
CLANG_CPPFLAGS = -DMESSAGE='"Compiled with Clang"'

ifeq ($(CXX), g++)
	CPPFLAGS += $(GCC_CPPFLAGS)
	CURL_TEST = curlcpp_test
else ifeq ($(CXX), clang++)
	CPPFLAGS += $(CLANG_CPPFLAGS)
	CURL_TEST = curl_test
endif

PROGRAMS  = integer_test fundamental_test arithmetic_test random_tests gmp_test_integer money_test valgrind_test floatrates_test $(CURL_TEST)
#PROGRAMS  = 

.PHONY: clean all

all: $(PROGRAMS)
	
LIBXMLNAME = pugixml
LIBXML = ${LIBXMLNAME}/lib${LIBXMLNAME}.a
LIBXML_HEADER_DIR = /home/mat/pugixml-1.11/src
IPATH= -I${LIBXML_HEADER_DIR}
LPATH= -L${LIBXMLNAME}
LIBS=  -l${LIBXMLNAME} -lcurl -lpthread

CURL_FILES=curl_interface.hpp curl_manager.hpp curl_downloader.hpp 

ifeq ($(CXX), g++)
	IPATH+=-I/home/mat/curlcpp/include
	LPATH+=-L/home/mat/curlcpp/build/src
	LIBS +=-lcurlcpp
	
	CURL_FILES+=curlcpp_downloader.hpp
else ifeq ($(CXX), clang++)
#	CURL_FILES+=curl_manager.hpp curl_downloader.hpp
endif

CPPFLAGS += -I${LIBXML_HEADER_DIR}

valgrind_test: directories ${LIBXML} valgrind_test.o floatrates.o network.o json_downloader.o asio_downloader.o floatrates_downloader.o xml_downloader.o floatrates_json.o floatrates_xml.o floatrates_test.o downloaders_test.o xml_processing.o system.o
	$(CXX) $(CPPFLAGS)  valgrind_test.o system.o ${LPATH} ${LIBS} -o valgrind_test
	
valgrind_test.o: valgrind_test.cpp floatrates.hpp network.hpp network.cpp floatrates.cpp asio_downloader.cpp asio_downloader.hpp json_downloader.hpp json_downloader.cpp floatrates_downloader.hpp floatrates_downloader.cpp xml_downloader.cpp xml_downloader.hpp floatrates_json.hpp floatrates_json.cpp floatrates_xml.hpp floatrates_xml.cpp floatrates_test.cpp floatrates_test.hpp downloaders_test.cpp xml_processing.cpp xml_processing.hpp system.cpp system.hpp
	$(CXX) $(CPPFLAGS) -c valgrind_test.cpp -o valgrind_test.o
	
system.o: system.cpp system.hpp
	$(CXX) $(CPPFLAGS) -c system.cpp -o system.o

$(CURL_TEST): directories ${CURL_TEST}.o ${LIBXML}
	${CXX} $(CPPFLAGS) ${CURL_TEST}.o ${LPATH} ${LIBS}  -o ${CURL_TEST}

${CURL_TEST}.o: ${CURL_TEST}.cpp ${CURL_FILES} 
	${CXX} -c $(CPPFLAGS) ${IPATH}  $< -o $@

directories:
	mkdir -p ${LIBXMLNAME}

${LIBXML}: ${LIBXMLNAME}/${LIBXMLNAME}.o
	ar rcs ${LIBXML} ${LIBXMLNAME}/${LIBXMLNAME}.o

${LIBXMLNAME}/${LIBXMLNAME}.o: ${LIBXML_HEADER_DIR}/${LIBXMLNAME}.cpp
	${CXX} -c $(CPPFLAGS) -I${LIBXML_HEADER_DIR}  $< -o $@

money_test: directories ${LIBXML} money_test.o integer.o integer_parsing.o money_init_test.o money.o money_operations_test.o floatrates.o network.o json_downloader.o asio_downloader.o money_currency_test.o downloaders_test.o floatrates_downloader.o floatrates_json.o xml_downloader.o floatrates_xml.o xml_processing.o
	$(CXX) $(CPPFLAGS) money_test.o integer.o integer_parsing.o money_init_test.o money.o money_operations_test.o floatrates.o network.o json_downloader.o asio_downloader.o money_currency_test.o downloaders_test.o floatrates_downloader.o floatrates_json.o xml_downloader.o floatrates_xml.o xml_processing.o ${LPATH} ${LIBS} -o money_test
	
floatrates_test: directories ${LIBXML} floatrates.o network.o json_downloader.o asio_downloader.o floatrates_downloader.o xml_downloader.o floatrates_json.o floatrates_xml.o floatrates_test.o downloaders_test.o xml_processing.o
	$(CXX) $(CPPFLAGS) floatrates.o network.o json_downloader.o asio_downloader.o floatrates_downloader.o xml_downloader.o floatrates_json.o floatrates_xml.o floatrates_test.o downloaders_test.o xml_processing.o ${LPATH} ${LIBS} -o floatrates_test

fundamental_test: fundamental_test.o integer.o integer_parsing.o parsing_test.o
	$(CXX) $(CPPFLAGS) -o fundamental_test fundamental_test.o integer.o integer_parsing.o parsing_test.o
	
arithmetic_test: arithmetic_test.o integer.o integer_parsing.o parsing_test.o
	$(CXX) $(CPPFLAGS) -o arithmetic_test arithmetic_test.o integer.o integer_parsing.o parsing_test.o
	
gmp_test_integer: integer.o integer_parsing.o command_line.o gmp_test_integer.o
	$(CXX) $(CPPFLAGS) -L/home/mat/gmp-6.2.1/.libs -lgmpxx -lgmp -o gmp_test_integer gmp_test_integer.o integer.o integer_parsing.o command_line.o
	
random_tests: random_tests.o integer.o integer_parsing.o command_line.o
	$(CXX) $(CPPFLAGS) -o random_tests random_tests.o integer.o integer_parsing.o command_line.o
	
integer_test: integer_test.o integer.o integer_parsing.o parsing_test.o
	$(CXX) $(CPPFLAGS) -o integer_test integer_test.o integer.o integer_parsing.o parsing_test.o
	
downloaders_test.o: downloaders_test.cpp json_downloader.hpp json_downloader.cpp network.hpp network.cpp asio_downloader.cpp asio_downloader.hpp xml_downloader.cpp xml_downloader.hpp floatrates_downloader.cpp floatrates_downloader.hpp
	$(CXX) $(CPPFLAGS) ${IPATH} -c downloaders_test.cpp
	
money_test.o: money_test.cpp money.cpp money_init_test.hpp money_init_test.cpp integers_extremums_tests.hpp money_utility_test.hpp money.hpp integer.hpp integer_parsing.hpp assertions.hpp integers_extremums_tests.hpp money_operations_test.hpp money_operations_test.cpp money_currency_test.hpp money_currency_test.cpp floatrates_json.hpp floatrates_json.cpp floatrates_downloader.hpp floatrates_downloader.cpp xml_downloader.cpp xml_downloader.hpp 
	$(CXX) $(CPPFLAGS) -c money_test.cpp
	
money_currency_test.o: money_currency_test.cpp money_currency_test.hpp money_init_test.cpp money_init_test.hpp money.cpp money.hpp integer.hpp integer_parsing.hpp assertions.hpp integers_extremums_tests.hpp money_utility_test.hpp money_utility.cpp
	$(CXX) $(CPPFLAGS) -c money_currency_test.cpp
	
money_operations_test.o: money_operations_test.cpp money_operations_test.hpp money_init_test.cpp money_init_test.hpp money.cpp money.hpp integer.hpp integer_parsing.hpp assertions.hpp integers_extremums_tests.hpp money_utility_test.hpp money_utility.cpp
	$(CXX) $(CPPFLAGS) -c money_operations_test.cpp
	
money_init_test.o: money_init_test.cpp money_init_test.hpp money.cpp money.hpp integer.hpp integer_parsing.hpp assertions.hpp integers_extremums_tests.hpp money_utility_test.hpp
	$(CXX) $(CPPFLAGS) -c money_init_test.cpp
	
money.o: money.cpp money.hpp money_utility.cpp floatrates.hpp network.hpp floatrates.cpp asio_downloader.cpp asio_downloader.hpp json_downloader.hpp json_downloader.cpp network.cpp floatrates_downloader.hpp floatrates_downloader.cpp floatrates_json.hpp floatrates_json.cpp floatrates_xml.hpp floatrates_xml.cpp xml_processing.hpp xml_processing.cpp
	$(CXX) $(CPPFLAGS) ${IPATH} -c money_utility.cpp -o money.o
	
xml_processing.o: xml_processing.hpp xml_processing.cpp
	$(CXX) $(CPPFLAGS) -c xml_processing.cpp -o xml_processing.o
	
floatrates_test.o: floatrates_test.cpp floatrates_test.hpp floatrates.hpp network.hpp network.cpp floatrates.cpp asio_downloader.cpp asio_downloader.hpp json_downloader.hpp json_downloader.cpp floatrates_downloader.hpp floatrates_downloader.cpp xml_downloader.cpp xml_downloader.hpp floatrates_json.hpp floatrates_json.cpp floatrates_xml.hpp floatrates_xml.cpp downloaders_test.cpp xml_processing.hpp xml_processing.cpp
	$(CXX) $(CPPFLAGS) ${IPATH} -c floatrates_test.cpp -o floatrates_test.o
	
floatrates_downloader.o: floatrates_downloader.cpp floatrates_downloader.hpp floatrates.hpp network.hpp network.cpp floatrates.cpp asio_downloader.cpp asio_downloader.hpp json_downloader.hpp json_downloader.cpp floatrates_json.cpp floatrates_json.hpp floatrates_xml.cpp floatrates_xml.hpp xml_processing.hpp xml_processing.cpp ${CURL_FILES}
	$(CXX) $(CPPFLAGS) ${IPATH} -c floatrates_downloader.cpp -o floatrates_downloader.o
	
asio_downloader.o: asio_downloader.hpp asio_downloader.cpp network.hpp network.cpp
	$(CXX) $(CPPFLAGS) -c asio_downloader.cpp
	
json_downloader.o: json_downloader.hpp json_downloader.cpp network.hpp network.cpp asio_downloader.cpp asio_downloader.hpp
	$(CXX) $(CPPFLAGS) -c json_downloader.cpp
	
floatrates_xml.o: floatrates.hpp floatrates.cpp floatrates_xml.hpp floatrates_xml.cpp xml_processing.hpp xml_processing.cpp
	$(CXX) $(CPPFLAGS) -c floatrates_xml.cpp
	
floatrates_json.o: floatrates.hpp floatrates.cpp floatrates_json.hpp floatrates_json.cpp
	$(CXX) $(CPPFLAGS) -c floatrates_json.cpp
	
floatrates.o: floatrates.hpp floatrates.cpp
	$(CXX) $(CPPFLAGS) -c floatrates.cpp
	
network.o: network.hpp network.cpp
	$(CXX) $(CPPFLAGS) -c network.cpp
	
fundamental_test.o: fundamental_test.cpp integer.hpp integer_parsing.hpp assertions.hpp parsing_test.hpp
	$(CXX) $(CPPFLAGS) -c fundamental_test.cpp
	
arithmetic_test.o: arithmetic_test.cpp integer.hpp integer_parsing.hpp parsing_test.hpp
	$(CXX) $(CPPFLAGS) -c arithmetic_test.cpp
	
gmp_test_integer.o: command_line.hpp command_line.cpp integer.hpp integer.cpp gmp_test_integer.cpp operations.hpp
	$(CXX) $(CPPFLAGS) -c gmp_test_integer.cpp
	
random_tests.o: random_tests.cpp integer.hpp random_numbers.hpp integer_parsing.hpp assertions.hpp command_line.hpp operations.hpp
	$(CXX) $(CPPFLAGS) -c random_tests.cpp
	
command_line.o: command_line.hpp
	$(CXX) $(CPPFLAGS) -c command_line.cpp
    
integer_test.o: integer_test.cpp integer.hpp integer_parsing.hpp assertions.hpp
	$(CXX) $(CPPFLAGS) -c integer_test.cpp
	
integer.o: integer.cpp integer.hpp integer_parsing.hpp
	$(CXX) $(CPPFLAGS) -c integer.cpp
	
integer_parsing.o: integer_parsing.cpp integer_parsing.hpp
	$(CXX) $(CPPFLAGS) -c integer_parsing.cpp
	
parsing_test.o: parsing_test.cpp parsing_test.hpp
	$(CXX) $(CPPFLAGS) -c parsing_test.cpp
	
clean :
	rm -rf ${LIBXMLNAME} $(PROGRAMS) *.o
