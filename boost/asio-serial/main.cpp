#include <boost/thread.hpp>
#include <iostream>

#include "buffered_async_serial.hpp"

using namespace std;
using namespace boost;

int main(int argc, char* argv[]) {
    try {
        buffered_async_serial serial("/dev/ttyUSB0", 115200 / 2);

        //Return immediately. String is written *after* the function returns, in a separate thread.
        serial.write_string("ls\r\n");

        //Simulate doing something else while the serial device replies.
        //When the serial device replies, the second thread stores the received data in a buffer.
        this_thread::sleep(posix_time::seconds(2));

        //Always returns immediately. If the terminator \r\n has not yet arrived, returns an empty string.
        cout << serial.read_string_until("\n") << endl;
        cout << serial.read_string_until("\n") << endl;
        cout << serial.read_string_until("\n") << endl;

        serial.close();

    } catch (boost::system::system_error& e) {
        cout << "Error: " << e.what() << endl;
        return 1;
    }
}
