# Copyright (C) 2024 Miro Palmu.
#
# This file is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This file is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this file.  If not, see <https://www.gnu.org/licenses/>.

single_source_examples = []

subdir('concept_demos')
subdir('wayland')

fs = import('fs')

# Note that what ordinal gets assigned to which example is not stable.
example_ordinal = 0
foreach example_source : single_source_examples
    name_prefix = 'eg@0@_'.format(example_ordinal) 
    example_name = name_prefix + fs.stem(example_source.full_path())
    executable(
        example_name,
        example_source,
        dependencies: [waylander_dep],
    )
    example_ordinal += 1
endforeach
