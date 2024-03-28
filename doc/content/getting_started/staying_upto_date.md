# Staying up to date

If you would like get the most recent version of the rxn-cpp project for your personal use you can perform the following operations

## Add an upstream git remote

!style halign=left
Add the main rxn-cpp repository as an "upstream" git remote, so that you can
[git fetch](https://git-scm.org/docs/git-fetch) updates from the main repository:

```bash
cd ~/projects/rxn-cpp
git remote add upstream https://github.com/gsgall/rxn-cpp
```

To fetch changes, run:

```bash
git fetch upstream
```

## Applying the changes to your local repo

!style halign=left
Now to apply these changes to your local and remote repository you can do the following


```bash
git rebase upstream
```

## Applying the changes to your remote repo

```bash
git push -f
```

## Rebuilding the 
