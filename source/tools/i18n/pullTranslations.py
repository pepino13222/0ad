#!/usr/bin/env python3
#
# Copyright (C) 2020 Wildfire Games.
# This file is part of 0 A.D.
#
# 0 A.D. is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# 0 A.D. is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with 0 A.D.  If not, see <http://www.gnu.org/licenses/>.

import os, sys

from txclib.project import Project

from i18n_helper import l10nToolsDirectory, projectRootDirectory

def main():
    l10nFolderName = "l10n"
    transifexClientFolder = ".tx"

    for root, folders, filenames in os.walk(projectRootDirectory):
        for folder in folders:
            if folder == l10nFolderName:
                if os.path.exists(os.path.join(root, folder, transifexClientFolder)):
                    path = os.path.join(root, folder)
                    os.chdir(path)
                    project = Project(path)
                    project.pull(fetchall=True, force=True)


if __name__ == "__main__":
    main()
