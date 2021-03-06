/******************************************************************************

   REBVO: RealTime Edge Based Visual Odometry For a Monocular Camera.
   Copyright (C) 2016  Juan José Tarrio
   
   Jose Tarrio, J., & Pedre, S. (2015). Realtime Edge-Based Visual Odometry
   for a Monocular Camera. In Proceedings of the IEEE International Conference
   on Computer Vision (pp. 702-710).
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

 *******************************************************************************/


 #ifndef EDGE_FINDER_H
#define EDGE_FINDER_H

#include "sspace.h"
#include "video_io.h"
#include "CircList.h"
#include "cam_model.h"
#include "image.h"

//Goal limits on Inverse Depth and Initialization
#define RHO_MAX     20
#define RHO_MIN     1e-3
#define RhoInit     1

//Maximun number of KeyLines to track for (in memory size)
#define KEYLINE_MAX     30000

struct KeyLine{

        int  p_inx;         //KeyLine linear index of image position


        Point2DF m_m;       //KeyLine's gradient vector
        Point2DF u_m;       //Normalized m_m
        float n_m;          //Norm of m_m

        Point2DF c_p;       //KeyLine's image position

        double rho;         //Estimated Inverse Depth
        double s_rho;       //Estimated Inverse Depth Uncertainty

        double rho0;        //Predicted Inverse Depth in EKF (use only if reescaling)
        double s_rho0;      //Predicted Inverse Depth Uncertainty


        Point2DF p_m;       //KL position in homogeneous coordinates (plane on focal length zf)

        Point2DF p_m_0;     //matched KL position in homogeneous coordinates

        int m_id;           //Id of the matching keyline
        int m_id_f;         //Id of the matching keyline by fordward matching

        int m_num;          //number of consecutive matches

        Point2DF m_m0;      //Grandient of matched KeyLine
        double n_m0;        //Norm of m_m0

        short int p_id;     //Id of previous consecutive KeyLine
        short int n_id;     //Id of next consecutive KeyLine
        short int net_id;   //Network ID of the KeyLine

};




class edge_finder
{

protected:

    cam_model &cam_mod;             //Pinhole Model of the Camera

    const Size2D &fsz;              //Wrapper for image size

    int max_img_value;              //Valor maximo de intensidad en las imagenes

    //**** Image Size Masks ****
    Image<int>img_mask_kl;          //pointer to Keyline Id in the image




    //**** KeyLine list ****
    KeyLine *kl;
    int kn;         //Number of KLs

public:




    edge_finder(cam_model &cam, float max_i_value);
    ~edge_finder();



    void detect(sspace *ss ,int plane_fit_size,double pos_neg_thresh,double dog_thresh,
                int kl_max, double &tresh, int &l_kl_num, int kl_ref=0, double gain=0, double thresh_max=1e10, double thresh_min=1-10);
    void build_mask(sspace *ss, int kl_max, int win_s, float per_hist, float grad_thesh, float dog_thesh);


    void join_edges();


    //***** Quick returns*****

    cam_model & GetCam(){return cam_mod;}

    int KNum(){return kn;}

    KeyLine & operator [](uint inx){return kl[inx];}    //Index operator to access keylines directly

    //Some iterators

    typedef KeyLine * iterator;
    typedef const KeyLine * const_iterator;

    iterator begin(){return &kl[0];}
    iterator end(){return &kl[kn];}

    void annotate_keyline(int inx);
};

#endif // EDGE_FINDER_H
