xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 18;
 0.00000;1.01954;4.83508;,
 -4.00414;1.01954;3.05484;,
 -4.16960;-1.01954;3.15715;,
 0.00000;-1.01954;4.75638;,
 4.00414;1.01954;3.05484;,
 4.16960;-1.01954;3.15715;,
 6.53622;1.01954;-0.10361;,
 6.33702;-1.01954;-0.36187;,
 4.46737;1.01954;-3.25340;,
 4.73748;-1.01954;-3.38302;,
 0.00000;1.01954;-4.83508;,
 0.00000;-1.01954;-4.78927;,
 -4.46737;1.01954;-3.25340;,
 -4.73748;-1.01954;-3.38302;,
 -6.53622;1.01954;-0.10361;,
 -6.33702;-1.01954;-0.36187;,
 -4.00414;1.01954;3.05484;,
 -4.16960;-1.01954;3.15715;;
 
 8;
 4;0,1,2,3;,
 4;4,0,3,5;,
 4;6,4,5,7;,
 4;8,6,7,9;,
 4;10,8,9,11;,
 4;12,10,11,13;,
 4;14,12,13,15;,
 4;16,14,15,17;;
 
 MeshMaterialList {
  4;
  8;
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  10;
  -0.624490;-0.003556;0.781025;,
  0.000000;0.021925;0.999760;,
  0.624490;-0.003557;0.781024;,
  0.816987;-0.027090;0.576019;,
  0.624067;0.056338;-0.779337;,
  0.000000;0.040561;-0.999177;,
  -0.624067;0.056339;-0.779337;,
  -0.858705;0.066849;-0.508092;,
  0.858705;0.066849;-0.508092;,
  -0.816987;-0.027089;0.576019;;
  8;
  4;1,0,0,1;,
  4;2,1,1,2;,
  4;3,2,2,3;,
  4;4,8,8,4;,
  4;5,4,4,5;,
  4;6,5,5,6;,
  4;7,6,6,7;,
  4;0,9,9,0;;
 }
 MeshTextureCoords {
  18;
  0.125000;0.666667;,
  0.000000;0.666667;,
  0.000000;0.333333;,
  0.125000;0.333333;,
  0.250000;0.666667;,
  0.250000;0.333333;,
  0.375000;0.666667;,
  0.375000;0.333333;,
  0.500000;0.666667;,
  0.500000;0.333333;,
  0.625000;0.666667;,
  0.625000;0.333333;,
  0.750000;0.666667;,
  0.750000;0.333333;,
  0.875000;0.666667;,
  0.875000;0.333333;,
  1.000000;0.666667;,
  1.000000;0.333333;;
 }
}
