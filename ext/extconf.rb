require 'mkmf'

if have_library('lzo2')
  create_makefile('lzoruby')
end
