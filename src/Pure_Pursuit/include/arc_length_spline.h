#ifndef MPCC_ARC_LENGTH_SPLINE_H
#define MPCC_ARC_LENGTH_SPLINE_H

#include "cubic_spline.h"
#include <map>
#include <vector>

static constexpr int N_SPLINE = 5000;

struct State
{
    double X;     // X轴位置
    double Y;     // Y轴位置
    double phi;   // 车身转角
    double vx;    // 车辆沿经线速度
    double vy;    // 车辆沿垂直经线方向速度
    double r;     // 角速度
    double s;     // 参数化轨迹弧长
    double D;     // 油门刹车
    double delta; // 前车轮角度
    double vs;    // 投影到轨迹线上的速度

    void setZero()
    {
        X = 0.0;
        Y = 0.0;
        phi = 0.0;
        vx = 0.0;
        vy = 0.0;
        r = 0.0;
        s = 0.0;
        D = 0.0;
        delta = 0.0;
        vs = 0.0;
    }

    void unwrap(double track_length)
    {
        if (phi > M_PI)
            phi -= 2.0 * M_PI;
        if (phi < -M_PI)
            phi += 2.0 * M_PI;

        if (s > track_length)
            s -= track_length;
        if (s < 0)
            s += track_length;
    }

    void vxNonZero(double vx_zero)
    {
        if (vx < vx_zero)
        {
            vx = vx_zero;
            vy = 0.0;
            r = 0.0;
            delta = 0.0;
        }
    }
};

// return value
struct RawPath
{
    Eigen::VectorXd X;
    Eigen::VectorXd Y;
};
// data struct
struct PathData
{
    Eigen::VectorXd X;
    Eigen::VectorXd Y;
    Eigen::VectorXd s;
    int n_points;
};

class ArcLengthSpline
{
public:
    // X and Y spline used for final spline fit
    void gen2DSpline(const Eigen::VectorXd &X, const Eigen::VectorXd &Y);
    Eigen::Vector2d getPostion(double) const;
    Eigen::Vector2d getDerivative(double) const;
    Eigen::Vector2d getSecondDerivative(double) const;
    double getLength() const;
    double porjectOnSpline(const State &x) const;

    ArcLengthSpline();

private:
    void setData(const Eigen::VectorXd &X_in, const Eigen::VectorXd &Y_in);
    void setRegularData(const Eigen::VectorXd &X_in, const Eigen::VectorXd &Y_in, const Eigen::VectorXd &s_in);
    Eigen::VectorXd compArcLength(const Eigen::VectorXd &X_in, const Eigen::VectorXd &Y_in) const;
    PathData resamplePath(const CubicSpline &initial_spline_x, const CubicSpline &initial_spline_y, double total_arc_length) const;
    RawPath outlierRemoval(const Eigen::VectorXd &X_original, const Eigen::VectorXd &Y_original) const;
    void fitSpline(const Eigen::VectorXd &X, const Eigen::VectorXd &Y);
    double unwrapInput(double x) const;

    PathData path_data_; // initial data and data used for successive fitting
                         //    PathData pathDataFinal; // final data
    CubicSpline spline_x_;
    CubicSpline spline_y_;
    double max_dist;
};

#endif // MPCC_ARC_LENGTH_SPLINE_H