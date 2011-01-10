require 'mkmf'

if have_header('lzo2')
  create_makefile('lzoruby')
end
