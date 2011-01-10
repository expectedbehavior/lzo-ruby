Gem::Specification.new do |spec|
  spec.name              = 'lzoruby'
  spec.version           = '0.1.3'
  spec.platform          = 'mswin32'
  spec.summary           = 'Ruby bindings for liblzo.'
  spec.require_paths     = %w(lib/i386-mswin32)
  spec.files             = %w(lib/i386-mswin32/lzoruby.so README LICENSE.txt ext/lzoruby.c)
  spec.author            = 'winebarrel'
  spec.email             = 'sgwr_dts@yahoo.co.jp'
  spec.homepage          = 'https://bitbucket.org/winebarrel/lzo-ruby'
  spec.has_rdoc          = true
  spec.rdoc_options      << '--title' << 'LZO/Ruby - Ruby bindings for LZO.'
  spec.extra_rdoc_files  = %w(README LICENSE.txt ext/lzoruby.c)
end
