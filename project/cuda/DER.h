#pragma once

#include <Eigen/Dense>

class DER {
public:
    int num_vertices;
    int num_edges;

    const double E; // Young's modulus
    const double G; // shear modulus

    const std::vector<double> a; // major radii of cross-section, edge has an elliptical cross-section
    const std::vector<double> b; // minor radii of cross-section
    
    const std::vector<Eigen::Vector3d> vertices;
    const std::vector<double> rest_lengths; // default lengths of edges
    const std::vector<Eigen::Vector2d> rest_curvatures; // default curvatures of vertices
    const std::vector<double> rest_twits; // default twist angles of edges

    std::vector<Eigen::Vector3d> edges; // vectors between vertices, the number of edges is one less than the number of vertices
    std::vector<Eigen::Vector3d> tangents; // tangent vectors
    std::vector<Eigen::Vector3d> ref_dir_1; // reference frame computed from geometry
    std::vector<Eigen::Vector3d> ref_dir_2;
    std::vector<Eigen::Vector3d> mat_dir_1; // material frame represented in comparison to reference frame
    std::vector<Eigen::Vector3d> mat_dir_2;
    std::vector<double> thetas; // twitst angle, difference between reference and material frame

    std::vector<double> k_Ss; // stretching stiffness associated with edges
    std::vector<double> betas; // bending stiffness associated with vertices
    std::vector<Eigen::Matrix2d> Bs; // twisting stiffness associated with vertices

    void initialize();
    void update(double dt);

private:
    void computeEdges();
    void updateReferenceFrame(); // update reference frame and tangents
    void updateMaterialFrame(); // compute material frame from reference frame
    double computeStretchingEnergy();
    double computeBendingEnergy();
    double computeTwistingEnergy();
    Eigen::Vector3d computeCurvature(int vertex_index);
    double computeTwist(int vertex_index);
    double computeVoronoiLength(int vertex_index);
};
