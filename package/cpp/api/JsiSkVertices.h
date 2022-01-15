#pragma once

#include "JsiSkHostObjects.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"

#include <SkVertices.h>

#pragma clang diagnostic pop

namespace RNSkia {

    using namespace facebook;

    class JsiSkVertices : public JsiSkWrappingSkPtrHostObject<SkVertices> {
    public:

        JsiSkVertices(std::shared_ptr<RNSkPlatformContext> context,
        sk_sp<SkVertices> vertices)
        : JsiSkWrappingSkPtrHostObject<SkVertices>(context, vertices) {}

        JSI_HOST_FUNCTION(bounds) {
            return jsi::Value::undefined();
        }

        JSI_HOST_FUNCTION(uniqueID) {
            return jsi::Value::undefined();
        }

        JSI_EXPORT_FUNCTIONS(JSI_EXPORT_FUNC(JsiSkVertices, bounds),
                             JSI_EXPORT_FUNC(JsiSkVertices, uniqueID), )

        /**
          Returns the underlying object from a host object of this type
         */
        static sk_sp<SkVertices> fromValue(jsi::Runtime &runtime,
                                                 const jsi::Value &obj) {
            const auto object = obj.asObject(runtime);
                return object
                        .asHostObject<JsiSkVertices>(runtime)
                        .get()
                        ->getObject();
        }

        /**
         * Creates the function for construction a new instance of the SkRect
         * wrapper
         * @param context platform context
         * @return A function for creating a new host object wrapper for the SkRect
         * class
         */
        static const jsi::HostFunctionType
        createCtor(std::shared_ptr<RNSkPlatformContext> context) {
            return JSI_HOST_FUNCTION_LAMBDA {
                auto mode = (SkVertices::VertexMode)arguments[0].getNumber();
                std::vector<SkPoint> positions;
                std::vector<SkPoint> texs;
                std::vector<SkColor> colors;
                std::vector<uint16_t> indices;

                auto jsiPositions = arguments[1].asObject(runtime).asArray(runtime);
                auto positionsSize = jsiPositions.size(runtime);
                for (int i = 0; i < positionsSize; i++) {
                    std::shared_ptr<SkPoint> point = JsiSkPoint::fromValue(
                            runtime, jsiPositions.getValueAtIndex(runtime, i).asObject(runtime));
                    positions.push_back(*point.get());
                }

                if (count >= 3 && !arguments[2].isNull() && !arguments[2].isUndefined()) {
                    auto jsiTexs = arguments[2].asObject(runtime).asArray(runtime);
                    auto texsSize = jsiTexs.size(runtime);
                    for (int i = 0; i < texsSize; i++) {
                        auto point = JsiSkPoint::fromValue(
                                runtime, jsiTexs.getValueAtIndex(runtime, i).asObject(runtime));
                        texs.push_back(*point.get());
                    }
                }

                if (count >= 4 && !arguments[3].isNull() && !arguments[3].isUndefined()) {
                    auto jsiColors = arguments[3].asObject(runtime).asArray(runtime);
                    auto colorsSize = jsiColors.size(runtime);
                    for (int i = 0; i < colorsSize; i++) {
                        SkColor color = jsiColors.getValueAtIndex(runtime, i).asNumber();
                        colors.push_back(color);
                    }
                }

                auto indicesSize = 0;
                if (count >= 5 && !arguments[4].isNull() && !arguments[4].isUndefined()) {
                    auto jsiIndices = arguments[4].asObject(runtime).asArray(runtime);
                    indicesSize = jsiIndices.size(runtime);
                    for (int i = 0; i < indicesSize; i++) {
                        uint16_t index = jsiIndices.getValueAtIndex(runtime, i).asNumber();
                        colors.push_back(index);
                    }
                }
                auto isVolatile = false;//count >= 6 && !arguments[5].isNull() && !arguments[5].isUndefined() ? true : arguments[5].getBool();
                auto flags = 0;
                // These flags are from SkVertices.h and should be kept in sync with those.
                if (texs.size() > 0) {
                    flags |= (1 << 0);
                }
                if (colors.size() > 0) {
                    flags |= (1 << 1);
                }
                if (!isVolatile) {
                    flags |= (1 << 2);
                }
                auto builder = SkVertices::Builder(mode, positionsSize, indicesSize, flags);
                auto vertices = SkVertices::MakeCopy(mode, positionsSize, positions.data(), texs.data(), colors.data(), indicesSize, indices.data());
                //auto vertices = builder.detach();
                return jsi::Object::createFromHostObject(
                        runtime, std::make_shared<JsiSkVertices>(context, vertices));
            };
        }
    };
} // namespace RNSkia