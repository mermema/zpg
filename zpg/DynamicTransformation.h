class DynamicTransformation {
public:
    virtual ~DynamicTransformation() = default;
    virtual void update(float time) = 0;
};
