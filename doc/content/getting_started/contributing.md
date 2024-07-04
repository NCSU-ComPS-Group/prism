PRISM is fully open source and we are open to outside contributions but ask that any contributers follow our development guidelines. Information in this page is intended to help facilitate this.

# PRISM Code Standards

For the most part PRISM follows the MOOSE code standards for all development. For information on file guidelines, naming conventions, example code, doxygen documentation, and other tips, please see [the MOOSE standard here](https://mooseframework.inl.gov/fenix/sqa/framework_scs.html). Some exceptions in the MOOSE standard that can be ignored are the sections: Index Variables in Looping Constructs, Trailing Whitespace and Tabs, Python.

# Creating a new environment

If you are interested in contributing you will need to download the [prism-dev](https://anaconda.org/gsgall/prism-dev) conda package to facilitate development. This package installs out dependcies and helps set environment variables needed or compilation. To create a new development evironment please use the following commands

```bash
  conda create -n prism-dev
```

```bash
  conda activate prism-dev
  conda config --add channels gsgall
  conda install prism-dev
```

## Creating and Referencing Issues

!style halign=left
When finding an issue with the code, or developing/suggesting a new feature or enhancement, an issue
should be created. This can be done ahead of time when developing code, or anytime before pushing
your changes. In any case, you will need to list an issue number in one of your commits! To get started:

1. Select “New issue” on this page: [https://github.com/NCSU-ComPS-Group/prism/issues](https://github.com/NCSU-ComPS-Group/prism/issues)
1. Select either “bug report” or “feature request” (most should be feature requests)
1. Provide issue descriptions of a couple of sentences (or as much as you see fit) following the prompts.
1. Create issue

You will now have an issue number next to the title. We will use it to link the pull request (PR) to the issue.

## Work in a GitHub fork

!style halign=left
PRISM development follows a "Fork & Pull" development process. See more information on
[forking repositories in the GitHub documentation](https://guides.github.com/activities/forking).

### Fork PRISM and clone

1. Navigate to [the PRISM repository](https://github.com/NCSU-ComPS-Group/prism)
1. Click the "Fork" button on the upper right of the code file tree to have a copy of FENIX in your own account
1. Clone your new fork to your local machine using the following command (this assumes you
   [have SSH set up on your GitHub account](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/adding-a-new-ssh-key-to-your-github-account)):

   ```
   cd ~/projects
   git clone git@github.com:<your_user_name>/prism.git
   ```

   In your local copy, this clone URL is given the shorthand name "origin". This will become relevant
   later in this document.

### Add an upstream git remote

!style halign=left
Add the main PRISM repository as an "upstream" git remote, so that you can
[git fetch](https://git-scm.org/docs/git-fetch) updates from the main repository:

```
cd ~/projects/prism
git remote add upstream git@github.com:idaholab/fenix.git
```

To fetch changes, run:

```
git fetch upstream
```

## Creating a new branch

Before make any changes locally (which would require having a working version of the application),
we will need to create a new branch.

1. In your terminal, go to your application:

   ```
   cd ~/projects/prism
   ```

1. It is usually best to make sure your changes are based on devel:

   ```
   git checkout devel
   ```

1. Clean and reset devel to get it up to date (make sure you do not have any changes on devel, because you would lose that):

   ```
   git reset --hard upstream/devel && git submodule update --init
   ```

1. Create new branch:

   ```
   git checkout -b <BRANCH_NAME>
   ```

## Make modifications and commit (the regular development workflow)

At this point, you have a local branch that is up to date with the upstream remote. You’re now ready
to make your changes. Remember, **small, consistent changes** are much better than dumping massive,
unrelated changes all at once.

1. Make your changes locally to address the issue (partly or fully)
1. In your terminal, go to your application:

   ```
   cd ~/projects/prism/test
   ```

1. Make sure that the app still compiles and that the tests run:

   ```
   make -j6 && ./run_tests -j6
   ```

   If you modified the documentation, you can then build and test it locally:

!alert note
Since we rely on [MooseDocs](https://mooseframework.inl.gov/python/MooseDocs/) for our documentation you will need to run these commands in the moose environment. Instructions on this can be found [here](https://mooseframework.inl.gov/getting_started/installation/conda.html)

   ```
    cd ~/projects/prism/doc/moose/test
    make hit
    cd ../../
    MOOSE_DIR=moose ROOT_DIR=./ ./moosedocs.py build --num-threads 4 --destination site
   ```

1. Type `git status` to see the status of your branch. That should show you the changed files and
   give you some commands to stage them.
1. Once you are ready, perform

   ```
   git add <FILE_NAME_1> <FILE_NAME_2> <FILE_NAME_3> ...
   ```

   to stage your updated files.
1. At that point, it is usually good practice to run

   ```
   git clang-format HEAD~1
   ```

   to fix the formatting of the files based on the `.clang-format` file in the root of the repository.
   If files are changed, run

   ```
   git add <...>
   ```

   for these files.
1. Now you’re ready to commit. Run

   ```
   git commit
   ```

1. A file editor will appear. Press the `i` key, and you’ll be able to type a short description of
   the commit. It is good practice to have a short title, and then a list of items describing the
   changes in more detail. At the bottom, you should add `(Ref. #<ISSUE_NUMBER>)`. It will look
   something like this:

   ```
   Updating this to enable that:
   - Update this file to do this
   - Update documentation to reflect that change
   - Create new test for new capability
   - Update existing gold files to reflect change

   (Ref. #1234)
   ```

1. To exit and save, press `esc`, then type `:wq`. The commit file will close.
1. You might have to do several commits to capture all your changes.

## Pushing and submitting PRISM changes

At some point, you'll be ready to push your work to your fork and submit the changes as a pull request
(PR). To do this follow the following steps.

1. To push your work to your fork (remember, it has the remote name `origin`) run:

   ```
   git push origin <BRANCH_NAME>
   ```

1. A link will be printed in the terminal; visit that page.
1. There, you’ll find instructions to create your PR. Fill out the form (one can simply copy what is
   in the commit messages, or write a custom summary of your own) and submit your PR!
1. To notify others (or the FENIX development team), you can also tag people in your PR to get their
   attention.
