
#include "src/base/Libraries.h"

#include "src/base/Window.h"

using namespace std;

int main() {

    auto* engineWindow = new Window();

    engineWindow -> Start();

    engineWindow -> Run();

    engineWindow -> Stop();

    return 0;
}
