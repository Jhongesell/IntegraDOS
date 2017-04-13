@echo off
pkzip Libreria.zip *.hpp *.cpp *.h *.dfm *.bat -rpex -sECVCYACLDMN
pkunzip Libreria.zip -t -sECVCYACLDMN
mkdir d:\libreria\libreria
copy Libreria.zip libreria
