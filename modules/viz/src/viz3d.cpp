#include <opencv2/viz/viz3d.hpp>
#include "viz3d_impl.hpp"


cv::viz::Viz3d::Viz3d(const String& window_name) : impl_(0) { create(window_name); }

cv::viz::Viz3d::Viz3d(const Viz3d& other) : impl_(other.impl_) 
{
    if (impl_) CV_XADD(&impl_->ref_counter, 1);
}

cv::viz::Viz3d& cv::viz::Viz3d::operator=(const Viz3d& other)
{
    if (this != &other)
    {
        release();
        impl_ = other.impl_;
        if (impl_) CV_XADD(&impl_->ref_counter, 1);
    }
    return *this;
}

cv::viz::Viz3d::~Viz3d() { release(); }

void cv::viz::Viz3d::create(const String &window_name)
{
    if (impl_) release();
    impl_ = new VizImpl(window_name);
    impl_->ref_counter = 1;
    // Register the window
    cv::viz::VizAccessor::getInstance().add(*this);
}

void cv::viz::Viz3d::release()
{
    // If the current referene count is equal to 2, we can delete it
    // - 2 : because minimum there will be two instances, one of which is in the map
    if (impl_ && CV_XADD(&impl_->ref_counter, -1) == 2)
    {
        // Erase the window
        cv::viz::VizAccessor::getInstance().remove(getWindowName());
        delete impl_;
        impl_ = 0;
    }
}

void cv::viz::Viz3d::spin() { impl_->spin(); }
void cv::viz::Viz3d::spinOnce (int time, bool force_redraw) { impl_->spinOnce(time, force_redraw); }
bool cv::viz::Viz3d::wasStopped() const { return impl_->wasStopped(); }

void cv::viz::Viz3d::registerKeyboardCallback(KeyboardCallback callback, void* cookie)
{ impl_->registerKeyboardCallback(callback, cookie); }

void cv::viz::Viz3d::registerMouseCallback(MouseCallback callback, void* cookie)
{ impl_->registerMouseCallback(callback, cookie); }

void cv::viz::Viz3d::showWidget(const String &id, const Widget &widget, const Affine3f &pose) { impl_->showWidget(id, widget, pose); }
void cv::viz::Viz3d::removeWidget(const String &id) { impl_->removeWidget(id); }
cv::viz::Widget cv::viz::Viz3d::getWidget(const String &id) const { return impl_->getWidget(id); }
void cv::viz::Viz3d::removeAllWidgets() { impl_->removeAllWidgets(); }
void cv::viz::Viz3d::setWidgetPose(const String &id, const Affine3f &pose) { impl_->setWidgetPose(id, pose); }
void cv::viz::Viz3d::updateWidgetPose(const String &id, const Affine3f &pose) { impl_->updateWidgetPose(id, pose); }
cv::Affine3f cv::viz::Viz3d::getWidgetPose(const String &id) const { return impl_->getWidgetPose(id); }

void cv::viz::Viz3d::setCamera(const Camera &camera) { impl_->setCamera(camera); }
cv::viz::Camera cv::viz::Viz3d::getCamera() const { return impl_->getCamera(); }
void cv::viz::Viz3d::setViewerPose(const Affine3f &pose) { impl_->setViewerPose(pose); }
cv::Affine3f cv::viz::Viz3d::getViewerPose() { return impl_->getViewerPose(); }

void cv::viz::Viz3d::resetCameraViewpoint (const String &id) { impl_->resetCameraViewpoint(id); }
void cv::viz::Viz3d::resetCamera() { impl_->resetCamera(); }

void cv::viz::Viz3d::convertToWindowCoordinates(const Point3d &pt, Point3d &window_coord) { impl_->convertToWindowCoordinates(pt, window_coord); }
void cv::viz::Viz3d::converTo3DRay(const Point3d &window_coord, Point3d &origin, Vec3d &direction) { impl_->converTo3DRay(window_coord, origin, direction); }

cv::Size cv::viz::Viz3d::getWindowSize() const { return impl_->getWindowSize(); }
void cv::viz::Viz3d::setWindowSize(const Size &window_size) { impl_->setWindowSize(window_size.width, window_size.height); }
cv::String cv::viz::Viz3d::getWindowName() const { return impl_->getWindowName(); }
void cv::viz::Viz3d::saveScreenshot (const String &file) { impl_->saveScreenshot(file); }
void cv::viz::Viz3d::setWindowPosition (int x, int y) { impl_->setWindowPosition(x,y); }
void cv::viz::Viz3d::setFullScreen (bool mode) { impl_->setFullScreen(mode); }
void cv::viz::Viz3d::setWindowName (const String &name) { impl_->setWindowName(name); }
void cv::viz::Viz3d::setBackgroundColor(const Color& color) { impl_->setBackgroundColor(color); }

void cv::viz::Viz3d::setRenderingProperty(int property, double value, const String &id) { getWidget(id).setRenderingProperty(property, value); }
double cv::viz::Viz3d::getRenderingProperty(int property, const String &id) { return getWidget(id).getRenderingProperty(property); }

void cv::viz::Viz3d::setDesiredUpdateRate(double time) { impl_->setDesiredUpdateRate(time); }
double cv::viz::Viz3d::getDesiredUpdateRate() { return impl_->getDesiredUpdateRate(); }

void cv::viz::Viz3d::setRepresentationToSurface() { impl_->setRepresentationToSurface(); }
void cv::viz::Viz3d::setRepresentationToWireframe() { impl_->setRepresentationToWireframe(); }
void cv::viz::Viz3d::setRepresentationToPoints() { impl_->setRepresentationToPoints(); }