single_source_examples = []

subdir('concept_demos')

fs = import('fs')

# Note that what ordinal gets assigned to which example is not stable.
example_ordinal = 0
foreach example_source : single_source_examples
    name_prefix = 'eg@0@_'.format(example_ordinal) 
    example_name = name_prefix + fs.stem(example_source.full_path())
    executable(
        example_name,
        example_source,
        dependencies: [guilander_dep],
    )
    example_ordinal += 1
endforeach
