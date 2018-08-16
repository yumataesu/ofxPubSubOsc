//
//  InstantiationList.inl
//
//  Created by ISHII 2bit on 2018/08/17.
//

INSTANTIATE_FUNCTION(bool);
INSTANTIATE_FUNCTION(char);
INSTANTIATE_FUNCTION(unsigned char);
INSTANTIATE_FUNCTION(short);
INSTANTIATE_FUNCTION(unsigned short);
INSTANTIATE_FUNCTION(int);
INSTANTIATE_FUNCTION(unsigned int);
INSTANTIATE_FUNCTION(long);
INSTANTIATE_FUNCTION(unsigned long);
INSTANTIATE_FUNCTION(long long);
INSTANTIATE_FUNCTION(unsigned long long);

INSTANTIATE_FUNCTION(float);
INSTANTIATE_FUNCTION(double);

INSTANTIATE_FUNCTION(std::string);
INSTANTIATE_FUNCTION(ofBuffer);

INSTANTIATE_FUNCTION(ofColor);
INSTANTIATE_FUNCTION(ofShortColor);
INSTANTIATE_FUNCTION(ofFloatColor);

INSTANTIATE_FUNCTION(ofVec2f);
INSTANTIATE_FUNCTION(ofVec3f);
INSTANTIATE_FUNCTION(ofVec4f);
INSTANTIATE_FUNCTION(ofQuaternion);
INSTANTIATE_FUNCTION(ofMatrix3x3);
INSTANTIATE_FUNCTION(ofMatrix4x4);

#ifdef GLM_VERSION
INSTANTIATE_FUNCTION(glm::vec2);
INSTANTIATE_FUNCTION(glm::vec3);
INSTANTIATE_FUNCTION(glm::vec4);
INSTANTIATE_FUNCTION(glm::quat);
INSTANTIATE_FUNCTION(glm::mat2);
INSTANTIATE_FUNCTION(glm::mat3);
INSTANTIATE_FUNCTION(glm::mat4);
#endif

INSTANTIATE_FUNCTION(ofRectangle);