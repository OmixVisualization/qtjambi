#!/bin/bash

for i in resources-linux32 resources-linux64 resources-win32 resources-win64 resources-mac32; do
	rm -rf ${i}/src/main/resources
done