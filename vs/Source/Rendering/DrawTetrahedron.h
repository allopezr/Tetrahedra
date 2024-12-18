#pragma once

#include "Model3D.h"

namespace Tet
{
   class DrawTetrahedron: public Model3D
   {
   protected:
       static float surface_area_dop_14(float dop[14]);

   public:
       DrawTetrahedron(Model3D& model);
       virtual ~DrawTetrahedron();

   protected:
       // dop14 with integer coordinates [-1, 0, 1]
       static float dot_dop14_n0(vec3 v)
       {
           return v.x;
       }

       static float dot_dop14_n1(vec3 v)
       {
           return v.y;
       }

       static float dot_dop14_n2(vec3 v)
       {
           return v.z;
       }

       static float dot_dop14_n3(vec3 v)
       {
           return (v.x + v.y + v.z);
       }

       static float dot_dop14_n4(vec3 v)
       {
           return (v.x + v.y - v.z);
       }

       static float dot_dop14_n5(vec3 v)
       {
           return (v.x - v.y + v.z);
       }

       static float dot_dop14_n6(vec3 v)
       {
           return (v.x - v.y - v.z);
       }

       static void dopInit(float dop[14])
       {
           dop[0] = FLT_MAX;
           dop[1] = -FLT_MAX;
           dop[2] = FLT_MAX;
           dop[3] = -FLT_MAX;
           dop[4] = FLT_MAX;
           dop[5] = -FLT_MAX;
           dop[6] = FLT_MAX;
           dop[7] = -FLT_MAX;
           dop[8] = FLT_MAX;
           dop[9] = -FLT_MAX;
           dop[10] = FLT_MAX;
           dop[11] = -FLT_MAX;
           dop[12] = FLT_MAX;
           dop[13] = -FLT_MAX;
       }

       static void dopInit(float dop[14], const vec3& v0, const vec3& v1, const vec3& v2)
       {
           dop[0] = std::min(dot_dop14_n0(v0), std::min(dot_dop14_n0(v1), dot_dop14_n0(v2)));
           dop[1] = std::max(dot_dop14_n0(v0), std::max(dot_dop14_n0(v1), dot_dop14_n0(v2)));
           dop[2] = std::min(dot_dop14_n1(v0), std::min(dot_dop14_n1(v1), dot_dop14_n1(v2)));
           dop[3] = std::max(dot_dop14_n1(v0), std::max(dot_dop14_n1(v1), dot_dop14_n1(v2)));
           dop[4] = std::min(dot_dop14_n2(v0), std::min(dot_dop14_n2(v1), dot_dop14_n2(v2)));
           dop[5] = std::max(dot_dop14_n2(v0), std::max(dot_dop14_n2(v1), dot_dop14_n2(v2)));
           dop[6] = std::min(dot_dop14_n3(v0), std::min(dot_dop14_n3(v1), dot_dop14_n3(v2)));
           dop[7] = std::max(dot_dop14_n3(v0), std::max(dot_dop14_n3(v1), dot_dop14_n3(v2)));
           dop[8] = std::min(dot_dop14_n4(v0), std::min(dot_dop14_n4(v1), dot_dop14_n4(v2)));
           dop[9] = std::max(dot_dop14_n4(v0), std::max(dot_dop14_n4(v1), dot_dop14_n4(v2)));
           dop[10] = std::min(dot_dop14_n5(v0), std::min(dot_dop14_n5(v1), dot_dop14_n5(v2)));
           dop[11] = std::max(dot_dop14_n5(v0), std::max(dot_dop14_n5(v1), dot_dop14_n5(v2)));
           dop[12] = std::min(dot_dop14_n6(v0), std::min(dot_dop14_n6(v1), dot_dop14_n6(v2)));
           dop[13] = std::max(dot_dop14_n6(v0), std::max(dot_dop14_n6(v1), dot_dop14_n6(v2)));
       }

       static void bvFitWithPoints(float dop[14], const vec3& v, vec3 points[14])
       {
           float d;
           d = dot_dop14_n0(v);
           dop[0] = std::min(dop[0], d);
           if (glm::epsilonEqual(d, dop[0], glm::epsilon<float>())) points[0] = v;
           dop[1] = std::max(dop[1], d);
           if (glm::epsilonEqual(d, dop[1], glm::epsilon<float>())) points[1] = v;

           d = dot_dop14_n1(v);
           dop[2] = std::min(dop[2], d);
           if (glm::epsilonEqual(d, dop[2], glm::epsilon<float>())) points[2] = v;
           dop[3] = std::max(dop[3], d);
           if (glm::epsilonEqual(d, dop[3], glm::epsilon<float>())) points[3] = v;

           d = dot_dop14_n2(v);
           dop[4] = std::min(dop[4], d);
           if (glm::epsilonEqual(d, dop[4], glm::epsilon<float>())) points[4] = v;
           dop[5] = std::max(dop[5], d);
           if (glm::epsilonEqual(d, dop[5], glm::epsilon<float>())) points[5] = v;

           d = dot_dop14_n3(v);
           dop[6] = std::min(dop[6], d);
           if (glm::epsilonEqual(d, dop[6], glm::epsilon<float>())) points[6] = v;
           dop[7] = std::max(dop[7], d);
           if (glm::epsilonEqual(d, dop[7], glm::epsilon<float>())) points[7] = v;

           d = dot_dop14_n4(v);
           dop[8] = std::min(dop[8], d);
           if (glm::epsilonEqual(d, dop[8], glm::epsilon<float>())) points[8] = v;
           dop[9] = std::max(dop[9], d);
           if (glm::epsilonEqual(d, dop[9], glm::epsilon<float>())) points[9] = v;

           d = dot_dop14_n5(v);
           dop[10] = std::min(dop[10], d);
           if (glm::epsilonEqual(d, dop[10], glm::epsilon<float>())) points[10] = v;
           dop[11] = std::max(dop[11], d);
           if (glm::epsilonEqual(d, dop[11], glm::epsilon<float>())) points[11] = v;

           d = dot_dop14_n6(v);
           dop[12] = std::min(dop[12], d);
           if (glm::epsilonEqual(d, dop[12], glm::epsilon<float>())) points[12] = v;
           dop[13] = std::max(dop[13], d);
           if (glm::epsilonEqual(d, dop[13], glm::epsilon<float>())) points[13] = v;
       }

       static void bvFit(float dop[14], float dopToFit[14])
       {
           dop[0] = std::min(dop[0], dopToFit[0]);
           dop[1] = std::max(dop[1], dopToFit[1]);
           dop[2] = std::min(dop[2], dopToFit[2]);
           dop[3] = std::max(dop[3], dopToFit[3]);
           dop[4] = std::min(dop[4], dopToFit[4]);
           dop[5] = std::max(dop[5], dopToFit[5]);
           dop[6] = std::min(dop[6], dopToFit[6]);
           dop[7] = std::max(dop[7], dopToFit[7]);
           dop[8] = std::min(dop[8], dopToFit[8]);
           dop[9] = std::max(dop[9], dopToFit[9]);
           dop[10] = std::min(dop[10], dopToFit[10]);
           dop[11] = std::max(dop[11], dopToFit[11]);
           dop[12] = std::min(dop[12], dopToFit[12]);
           dop[13] = std::max(dop[13], dopToFit[13]);
       }

       static void getFace(float dop[14], int faceId, vec3& direction, float& d)
       {
           static const vec3 DOP14_NORMAL[7] = {
            { 1.f, 0.f, 0.f },
            { 0.f, 1.f, 0.f },
            { 0.f, 0.f, 1.f },

            { 0.57735f, 0.57735f, 0.57735f },
            { 0.57735f, 0.57735f, -0.57735f },
            { 0.57735f, -0.57735f, 0.57735f },
            { 0.57735f, -0.57735f, -0.57735f },
           };

           const int di = (faceId >> 1);
           if ((faceId & 1) != 0)
           {
               direction = DOP14_NORMAL[di];
               d = -dop[faceId];
           }
           else
           {
               direction = -DOP14_NORMAL[di];
               d = dop[faceId];
           }
       }
   };
}
