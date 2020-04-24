///////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2010, Jason Mora Saragih, all rights reserved.
//
// This file is part of FaceTracker.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//
//     * The software is provided under the terms of this licence stricly for
//       academic, non-commercial, not-for-profit purposes.
//     * Redistributions of source code must retain the above copyright notice, 
//       this list of conditions (licence) and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright 
//       notice, this list of conditions (licence) and the following disclaimer 
//       in the documentation and/or other materials provided with the 
//       distribution.
//     * The name of the author may not be used to endorse or promote products 
//       derived from this software without specific prior written permission.
//     * As this software depends on other libraries, the user must adhere to 
//       and keep in place any licencing terms of those libraries.
//     * Any publications arising from the use of this software, including but
//       not limited to academic journal and conference publications, technical
//       reports and manuals, must cite the following work:
//
//       J. M. Saragih, S. Lucey, and J. F. Cohn. Face Alignment through 
//       Subspace Constrained Mean-Shifts. International Conference of Computer 
//       Vision (ICCV), September, 2009.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED 
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO 
// EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF 
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///////////////////////////////////////////////////////////////////////////////
#include <FaceTracker/FDet.h>
#include <opencv2/imgproc.hpp>
using namespace FACETRACKER;
using namespace std;
//===========================================================================
FDet::~FDet()
{  
}
//===========================================================================
FDet& FDet::operator= (FDet const& rhs)
{
  this->_min_neighbours = rhs._min_neighbours;
  this->_min_size = rhs._min_size;
  this->_img_scale = rhs._img_scale;
  this->_scale_factor = rhs._scale_factor;
  this->_cascade = rhs._cascade;
  this->small_img_ = rhs.small_img_.clone(); return *this;
}
//===========================================================================
void FDet::Init(const char* fname,
		const double img_scale,
		const double scale_factor,
		const int    min_neighbours,
		const int    min_size)
{
  if(!_cascade.load(fname)){
    printf("ERROR(%s,%d) : Failed loading classifier cascade!\n",
	   __FILE__,__LINE__); abort();
  }
  std::ifstream file_stream;
  file_stream.open(fname);
  cascade_conf << file_stream.rdbuf();
  _img_scale      = img_scale;
  _scale_factor   = scale_factor;
  _min_neighbours = min_neighbours;
  _min_size       = min_size; return;
}
//===========================================================================
cv::Rect FDet::Detect(cv::Mat im)
{
  assert(im.type() == CV_8U);
  cv::Mat gray; int i,maxv; cv::Rect R;
  int w = cvRound(im.cols/_img_scale);
  int h = cvRound(im.rows/_img_scale);
  if((small_img_.rows!=h) || (small_img_.cols!=w))small_img_.create(h,w,CV_8U);
  if(im.channels() == 1)gray = im;
  else{gray=cv::Mat(im.rows,im.cols,CV_8U);cv::cvtColor(im,gray,cv::COLOR_BGR2GRAY);}
  cv::resize(gray,small_img_,cv::Size(w,h),0,0,cv::INTER_LINEAR);
  cv::equalizeHist(small_img_,small_img_);

  std::vector<cv::Rect> objects;
  _cascade.detectMultiScale(small_img_, objects,
          _scale_factor, _min_neighbours, 0,
          cv::Size(_min_size, _min_size));

  if(objects.empty())return cv::Rect(0,0,0,0);
  for(i = 0,maxv = 0; i < objects.size(); i++){
    const auto& r = objects[i];
    if(i == 0 || maxv < r.width*r.height){
      maxv = r.width*r.height; R.x = r.x*_img_scale; R.y = r.y*_img_scale;
      R.width  = r.width*_img_scale; R.height = r.height*_img_scale;
    }
  }
  return R;
}
//===========================================================================
void FDet::Load(const char* fname)
{
  ifstream s(fname); assert(s.is_open()); this->Read(s); s.close(); return;
}
//===========================================================================
void FDet::Save(const char* fname)
{
  ofstream s(fname); assert(s.is_open()); this->Write(s);s.close(); return;
}
//===========================================================================
void FDet::Write(ofstream &s)
{
  // TODO check if this works
  s << IO::FDET                          << " "
    << _min_neighbours                   << " " 
    << _min_size                         << " "
    << _img_scale                        << " "
    << _scale_factor                     << " "
    << cascade_conf.str();
  return;
}
//===========================================================================
void FDet::Read(ifstream &s,bool readType)
{ 
  // TODO check if this works
  if(readType){int type; s >> type; assert(type == IO::FDET);}
  s >> _min_neighbours >> _min_size >> _img_scale >> _scale_factor;
  cascade_conf << s.rdbuf();
  return;
}
//===========================================================================
