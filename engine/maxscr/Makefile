# Makefile for max sciprt distribution to max scirits directory
# by zho@korea.com
# 2002-11-01-01
# $Id: Makefile,v 1.1.1.1 2002/09/06 00:44:05 Jiho Choi Exp $

OUTPUTDIR=c:/3dsmax5/scripts/znzin

all : ${OUTPUTDIR}/znModel.ms ${OUTPUTDIR}/znObjectViewer.ms ${OUTPUTDIR}/znOBBox.ms

${OUTPUTDIR}/znObjectViewer.ms : znObjectViewer.ms
	cp znObjectViewer.ms ${OUTPUTDIR}/znObjectViewer.ms
	
${OUTPUTDIR}/znModel.ms : znModel.ms
	cp znModel.ms ${OUTPUTDIR}/znModel.ms

${OUTPUTDIR}/znOBBox.ms : znOBBox.ms
	cp znOBBox.ms ${OUTPUTDIR}/znOBBox.ms

clean :
	rm -f ${OUTPUTDIR}/*.ms
