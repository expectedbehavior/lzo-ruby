Gem::Specification.new do |spec|
  spec.name              = 'lzoruby'
  spec.version           = '0.1.3'
  spec.summary           = 'Ruby bindings for LZO.'
  spec.files             = Dir.glob('ext/**/*.*') + %w(ext/extconf.rb README LICENSE.txt)
  spec.author            = 'winebarrel'
  spec.email             = 'sgwr_dts@yahoo.co.jp'
  spec.homepage          = 'https://bitbucket.org/winebarrel/lzo-ruby'
  spec.extensions        = 'ext/extconf.rb'
  spec.has_rdoc          = true
  spec.rdoc_options      << '--title' << 'LZO/Ruby - Ruby bindings for LZO.'
  spec.extra_rdoc_files  = %w(README LICENSE.txt ext/lzoruby.c)
end
