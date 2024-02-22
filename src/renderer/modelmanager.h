#ifndef __MODEL_MANAGER_H__
#define __MODEL_MANAGER_H__

namespace CG {
    class RenderModel;

    class ModelManager {
    public:
        ModelManager();
        ~ModelManager();
    
        void Init();
        void Shutdown();

        RenderModel *GetModel(const char *filename);
    };
}

#endif