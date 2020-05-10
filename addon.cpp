#include <napi.h>
#include <windows.h>
#include <chrono>
#include <iostream>
#include <thread>

typedef void (*FORM_t)(void (*)());

using namespace Napi;

std::thread addonThread;
std::thread dllThread;

ThreadSafeFunction tsfn;

Value MyWinMain(const CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 2) {
        throw TypeError::New(env, "Expected two arguments");
    } else if (!info[0].IsFunction()) {
        throw TypeError::New(env, "Expected first arg to be function");
    } else if (!info[1].IsNumber()) {
        throw TypeError::New(env, "Expected second arg to be number");
    }

    int count = info[1].As<Number>().Int32Value();

    tsfn = ThreadSafeFunction::New(
        env,
        info[0].As<Function>(),  // JavaScript function called asynchronously
        "Resource Name",         // Name
        0,                       // Unlimited queue
        1,                       // Only one thread will use this initially
        [](Napi::Env) {          // Finalizer used to clean threads up
            addonThread.join();
        });

    dllThread = std::thread([&] {
        HMODULE WINAPI myDLL = LoadLibrary("C++ Builder Project\\Win64\\Release\\Project1.dll");

        auto buttonClick = []() {
            napi_status status = tsfn.BlockingCall(new std::string("Click!"), [](Napi::Env env, Function jsCallback, std::string* value) {
                jsCallback.Call({String::New(env, *value)});
                delete value;
            });
        };

        ((FORM_t)GetProcAddress(myDLL, "ShowForm"))(buttonClick);
    });

    addonThread = std::thread([count] {
        auto callback = [](Napi::Env env, Function jsCallback, std::string* value) {
            jsCallback.Call({String::New(env, *value)});
            delete value;
        };

        for (int i = 0; i < count; i++) {
            napi_status status = tsfn.BlockingCall(new std::string("Tock!"), callback);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        tsfn.Release();
    });

    return Boolean::New(env, true);
}

Napi::Object Init(Napi::Env env, Object exports) {
    exports.Set("MyWinMain", Function::New(env, MyWinMain));
    return exports;
}

NODE_API_MODULE(clock, Init)