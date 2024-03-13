#include "modelmanager.h"
#include "model.h"
#include "../shared.h"
#include "../utility/str.h"

#include "model_local.h"

namespace CG {
    ModelManager::ModelManager() {
    }

    ModelManager::~ModelManager() {
    }

    void ModelManager::Init() {
    }

    void ModelManager::Shutdown() {
    }

    RenderModel *ModelManager::LoadModel(const char *filename) {
        RenderModel *model;

        const char *ext = Str::FileExtension(filename);

        if (Str::EqualTo(ext, "txt")) {
            model = new RenderModelSimple();
        }
        else if (Str::EqualTo(ext, "obj")) {
            model = new RenderModelObj();
        }
        else {
            assert(false);
            return NULL;
        }

        model->InitFromFile(filename);

        return model;
    }
} 
