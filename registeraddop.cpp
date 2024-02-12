// RegisterAddOp.cpp
#include "addop.h"

// Assuming AddOp is a class that inherits from Ort::CustomOpApi
class AddOp; // Define your AddOp class accordingly

extern "C" {
  OrtStatus* ORT_API_CALL RegisterCustomOps(OrtSessionOptions* options, const OrtApiBase* api_base) {
    // Directly use the C API to get the global API instance
    const OrtApi* api = OrtGetApiBase()->GetApi(ORT_API_VERSION);

    OrtCustomOpDomain* domain = nullptr;
    OrtStatus* status = api->CreateCustomOpDomain("MyCustomDomain", &domain);
    if (status != nullptr) {
      return status; // Handle error
    }

    // Assuming AddOp is properly defined and compatible
    AddOp* addOp = new AddOp();
    OrtCustomOp* customOp = reinterpret_cast<OrtCustomOp*>(addOp); // Direct casting, make sure this is safe!

    if (api->CustomOpDomain_Add(domain, customOp) != nullptr) {
        api->ReleaseCustomOpDomain(domain); // Clean up on error
        return nullptr; // Error handling
    }

    status = api->AddCustomOpDomain(options, domain);
    // Do not release domain here; it will be released with the session options
    return status; // Return status to check for errors
  }
}
