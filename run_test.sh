#!/bin/bash
emacs --batch \
      -L $(pwd) \
      -l test.el
